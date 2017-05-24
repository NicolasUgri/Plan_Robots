/*
 * createMap.cc
 *
 * Converts a bitmap into an occupancy map where 1 implies 
 * that the cell is occupied and 0 implies it is free. This  
 * is used to identify obstacles in the environement.
 */

#include "createMap.h"

using namespace PlayerCc;

int** obstacleMap::getMap(playerc_client_t *client) {
	try {
		printf("Creating Map\n");
		playerc_map_t *map = playerc_map_create(client, 0);
		if (map == NULL) {
			printf("map is NULL\n");
			return 0;
		}

		fprintf(stderr, "Subscribing map...\n");

		if (playerc_map_subscribe(map, PLAYERC_OPEN_MODE) != 0) {
			fprintf(stderr, "Error subscribing: %s\n", playerc_error_str());
			return 0;
		}

		/* Request map data */
		fprintf(stderr, "Get map...\n");
		playerc_map_get_map(map);

		fprintf(stderr,
				"Map info: resolution: %f, width=%d, height=%d, ox: %f, oy: %f\n",
				map->resolution, map->width, map->height, map->origin[0],
				map->origin[1]);

		width_ = map->width;
		height_ = map->height;
		resolution_ = map->resolution;

		map_ = new int*[height_];

		int row = -1, col = 0, cellValue = 0;

		for (int i = 0; i < width_ * height_; i++, col++) {
			if (i % width_ == 0) {
				row++;
				col = 0;
				map_[row] = new int[width_];
			}

			if (map->cells[i] == 1)
				cellValue = 1;
			else
				cellValue = 0;

			map_[row][col] = cellValue;
		}
	} catch (PlayerError& e) {
		std::cerr << e << std::endl;
		return 0;
	}

//	  for (int k = 0; k < height_; k++){
//		  for (int j = 0; j < width_; j++){
//			  if (j%400 == 0 && j+k!= 0) printf("%d\n",map_[k][j] );
//			  else printf("%d",map_[k][j] );
//		  }
//	  }

	return map_;
}

int obstacleMap::width() {
	return width_;
}

int obstacleMap::height() {
	return height_;
}

int obstacleMap::size() {
	return width_ * height_;
}

double obstacleMap::resolution() {
	return resolution_;
}

int** obstacleMap::map() {
	return map_;
}

obstacleMap::~obstacleMap() {
	for (int i = 0; i < height_; i++)
		delete map_[i];

	delete[] map_;
}

