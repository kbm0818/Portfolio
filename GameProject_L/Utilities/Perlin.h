#pragma once

class Perlin {
public:
	// Initialize with the reference values for the permutation vector
	Perlin();
	// Generate a new permutation vector based on the value of seed
	Perlin(unsigned int seed);
	// Get a noise value, for 2D images z can have any value
	double noise(double x, double y, double z = 0.0);
private:
	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);

private:
	// The permutation vector
	vector<int> p;
};
