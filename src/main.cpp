#include <iostream>
#include <algorithm>
#include <fstream> 
#include <string>
#include <thread>
#include "color.h"
#include "rt_utility.h"

#include "hittable_list.h"
#include "sphere.h"

#include "camera.h"

#include "SDL.h"

vec3 random_in_unit_sphere() {
    while (true) {
        auto p = random(-1,1);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

color ray_color(const ray& r, const hittable& world, int depth) {
	hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0,0,0);
		
	if (world.hit(r, 0, infinity, rec)) {
        point3 target = rec.p + rec.normal + random_in_unit_sphere();
        return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth-1);
	}

	vec3 unit_direction = unit_vector(r.get_direction());
	auto t = 0.5*(unit_direction.y() + 1.0);
	return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

int main(int argc, char** argv) 
{
	// Image Constants
	constexpr int IMAGE_WIDTH = 500;
	constexpr auto ASPECT_RATIO = 16.0 / 9.0;
	constexpr int IMAGE_HEIGHT = static_cast<int>(IMAGE_WIDTH / ASPECT_RATIO);

    constexpr int SAMPLES_PER_PIXEL = 100;
    constexpr int MAX_DEPTH = 50;

	// World
	hittable_list world;
	world.add(std::make_shared<sphere>(point3(0,0,-1), 0.5));
	world.add(std::make_shared<sphere>(point3(0,-100.5,-1), 100));

	// Initialize Camera
	Camera default_cam (
		IMAGE_WIDTH,
		ASPECT_RATIO,
		2.0,
		1.0,
		point3(0, 0, 0)
	);

	// intialize sdl subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "Failed to initialize the SDL2 library\n";

		return -1;
	}

	SDL_Window *window = SDL_CreateWindow(
		"raytacing demo",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		IMAGE_WIDTH, IMAGE_HEIGHT,
		SDL_WINDOW_RESIZABLE);
		
	if (!window)
	{
		std::cout << "Failed to create window\n";
		return -1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

	std::cout << "Lower Left Corner: \n" 
			<< "x = " << default_cam.get_lower_left_corner().x()
			<< "\n" << "y = " << default_cam.get_lower_left_corner().y() << "\n" 
			<< "z = " << default_cam.get_lower_left_corner().z() << "\n";

	std::ofstream output_file;
	output_file.open([&](){
		if (argc > 1) {
			const char * p1 = argv[1];
			return p1;
		} else {
			return "default.ppm";
		}
	}());

	output_file << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";

	SDL_Texture *texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING, IMAGE_WIDTH, IMAGE_HEIGHT);

	Uint32 pixels[IMAGE_WIDTH * IMAGE_HEIGHT] = {0};

	std::thread concurrent_render([&](){
	//		std::fill_n(pixels, IMAGE_WIDTH * IMAGE_HEIGHT, 0xA3C8FF);
	//		top left to right then down
		for (int j = IMAGE_HEIGHT-1; j >= 0; --j) {
			std::cerr << "\r Scanlines remaining:" << j << ' ' << std::flush;

			for (int i = 0; i < IMAGE_WIDTH; ++i) {
					color pixel_color(0,0,0);
					for (int s = 0; s < SAMPLES_PER_PIXEL; ++s) {
						auto u = (i + random_double()) / (IMAGE_WIDTH-1);
						auto v = (j + random_double()) / (IMAGE_HEIGHT-1);
						ray r = default_cam.get_ray(u,v);
                		pixel_color += ray_color(r, world, MAX_DEPTH);
					} 
					pixels[i + (j*IMAGE_WIDTH)] = write_color(pixel_color, SAMPLES_PER_PIXEL);
					write_color_to_file(output_file, pixel_color, SAMPLES_PER_PIXEL);
				}
			}
	});

	for (bool interrupt = false; !interrupt;)
	{
		SDL_Event ev;
		while (SDL_PollEvent(&ev))
			switch (ev.type)
			{
			case SDL_QUIT:
				interrupt = true;
				break;
			}
		// render pixels
		SDL_UpdateTexture(texture, nullptr, pixels, IMAGE_WIDTH * 4);
		SDL_RenderCopy(renderer, texture, nullptr, nullptr);
		SDL_RenderCopyEx(renderer, texture, nullptr, nullptr, 0, nullptr, SDL_FLIP_VERTICAL);
		SDL_RenderPresent(renderer);
	}
	
	output_file.close();
	concurrent_render.join();

	std::cerr << "\nDone.\n";
	
	//test
	std::cout   << "ARGC COUNT: " << argc << "\n" 
				<< "ARGV COUNT: " << argv[1];

	SDL_Delay(1000 / 60);

	return 0;
}