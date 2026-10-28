#pragma once

class Constant
{
public:
	static constexpr char WALL_BLOCK = 'x';
	static constexpr const char EMPTY_BLOCK = 'o';
	static constexpr const char HERO_BLOCK = 'h';
	static constexpr const char VILLAIN_BLOCK = 'v';
	static constexpr int BLOCK_SIZE = 10;

	// Tamaño original de las mallas base (cube.mesh y el cubo subdividido miden 100)
	static constexpr float MESH_SIZE = 100.0f;
	// Factor para que una malla de MESH_SIZE ocupe exactamente un bloque
	static constexpr float MESH_TO_BLOCK = BLOCK_SIZE / MESH_SIZE;
};

