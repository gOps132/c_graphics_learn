#include <iostream>
#include <algorithm>
#include <fstream> 
#include <string>

#include "color.h"
#include "rt_utility.h"

#include "hittable_list.h"
#include "sphere.h"

#include "camera.h"

#include "SDL.h"

color ray_color(const ray& r, const hittable& world) {
	hit_record rec;
	if (world.hit(r, 0, infinity, rec)) {
		return 0.5 * (rec.normal + color(1,1,1));
	}
	vec3 unit_direction = unit_vector(r.get_direction());
	auto t = 0.5*(unit_direction.y() + 1.0);
	return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

unsigned long createRGBA(int r, int g, int b, int a)
{   
    return ((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8)
           + (a & 0xff);
}

int main(int argc, char** argv) 
{
	constexpr int IMAGE_WIDTH = 200;

	// Image
	constexpr auto ASPECT_RATIO = 16.0 / 9.0;
	constexpr int IMAGE_HEIGHT = static_cast<int>(IMAGE_WIDTH / ASPECT_RATIO);

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
		IMAGE_WIDTH * 4, IMAGE_HEIGHT * 4,
		SDL_WINDOW_RESIZABLE);
		
	if (!window)
	{
		std::cout << "Failed to create window\n";
		return -1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

	// World
	hittable_list world;
	world.add(std::make_shared<sphere>(point3(0,0,-1), 0.5));
	world.add(std::make_shared<sphere>(point3(0,-100.5,-1), 100));

	Camera default_cam (
		IMAGE_WIDTH,
		ASPECT_RATIO,
		2.0,
		1.0,
		point3(0, 0, 0)
	);

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
	output_file << "P3\n" << default_cam.get_image_width() << ' ' << default_cam.get_image_height() << "\n255\n";

	SDL_Texture *texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING, IMAGE_WIDTH, IMAGE_HEIGHT);

	// Render
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

		Uint32 pixels[IMAGE_WIDTH * IMAGE_HEIGHT] = {0};

		// std::fill_n(pixels, IMAGE_WIDTH * IMAGE_HEIGHT, 0xA3C8FF);

// 		render pixels
//		right to left or left o right?
		for (int j = default_cam.get_image_height()-1; j >= 0; --j) {
			// check if image is done rendering 
			std::cerr << "\r Scanlines remaining:" << j << ' ' << std::flush;

			for (int i = 0; i < default_cam.get_image_width(); ++i) {
				auto u = double(i) / (default_cam.get_image_width()-1);
				auto v = double(j) / (default_cam.get_image_height()-1);

				ray r(default_cam.get_origin(),
					default_cam.get_lower_left_corner() + 
					u*default_cam.get_horizontal() + 
					v*default_cam.get_vertical() - 
					default_cam.get_origin()
				);
				color pixel_color = ray_color(r, world);

				// TODO: convert rgb vector to hexadecimal cast to integer
				// TODO: Map pixel color to pixels properly
				
				// pixels[j+i] = createRGBA(
				// 	(int)pixel_color.x(),
				// 	(int)pixel_color.y(),
				// 	(int)pixel_color.z(),
				// 	0
				// );

				pixels[i] = (int)0xA3C8FF;

				// write to file for debug
				write_color(output_file, pixel_color);
			}
		}

		SDL_UpdateTexture(texture, nullptr, pixels, IMAGE_WIDTH * 4);
		SDL_RenderCopy(renderer, texture, nullptr, nullptr);
		SDL_RenderPresent(renderer);

	}
	output_file.close();
	std::cerr << "\nDone.\n";
	
	//test
	std::cout   << "ARGC COUNT: " << argc << "\n" 
				<< "ARGV COUNT: " << argv[1];

	SDL_Delay(1000 / 60);

	return 0;
}