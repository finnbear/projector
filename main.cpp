#include <fstream>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

long resolution = 8192;
const char *filename = "out.ppm";

double sample2D(double x, double y, int iterations) {
	x *= 2;
	y *= 2;

	double x0 = x;
	double y0 = y;

	double t_2 = 4;
	double sX = 0;
	double sY = 0;

	for (int i = 0; i < iterations; i++) {
		if (x * x + y * y > t_2) {
			return (double)i / iterations;
		}

		double x1 = x;
		double y1 = y;

		x = x1 * x1 - y1 * y1 + x0 + sX;
		y = x1 * y1 + y1 * x1 + y0 + sY;
	}

	return 1.0;
}

double multiSample2D(double x, double y, int iterations) {
	double sum = 0;

	double space = 1e-10;

	sum += sample2D(x + 0, y + 0, iterations);
	sum += sample2D(x + space, y - space, iterations);
	sum += sample2D(x + space, y + space, iterations);
	sum += sample2D(x - space, y + space, iterations);
	sum += sample2D(x + space, y + space, iterations);

	return sum / 5;
}

int main()
{
	std::ofstream image(filename);

	image << "P3" << std::endl;
	image << resolution << " " << resolution << std::endl;
	image << "255" << std::endl;

	int percent = 0;

	for (long pixelY = 0; pixelY < resolution; pixelY++) {
		for (long pixelX = 0; pixelX < resolution; pixelX++) {
			double x = ((double)pixelX / resolution - 0.5) * 2;
			double y = ((double)pixelY / resolution - 0.5) * 2;

			int r = multiSample2D(x, y, 60) * 255;
			int g = multiSample2D(x, y, 30) * 255;
			int b = multiSample2D(x, y, 15) * 255;

			image << r << " " << g << " " << b << std::endl;
		}

		if ((int)((double)pixelY / resolution * 100) > percent) {
			std::cout << ++percent << "%" << std::endl;
		}
	}

	image.close();
}