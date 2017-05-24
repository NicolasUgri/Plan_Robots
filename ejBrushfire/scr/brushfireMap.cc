#include "brushfireMap.h"

double** brushfireMap::generate(obstacleMap& map) {
	width_ = map.width();
	height_ = map.height();

	map_ = new double*[height_];

	for (int i = 0; i < height_; i++) {
		map_[i] = new double[width_];
		for (int j = 0; j < width_; j++)
			map_[i][j] = map.map()[i][j];
	};

	bool free_cell = true;
	int iters = 1;
	while (free_cell) {
		free_cell = false;

		for (int i = 0; i < height_; i++) {
			for (int j = 0; j < width_; j++) {
				if (map_[i][j] == 0)
					free_cell = true;
				else if (map_[i][j] == iters)
					updateCellsAround(map_[i][j] + 1, i, j);
			}
		}
		iters++;
	}
	return map_;
}

void brushfireMap::updateCellsAround(double value, int i, int j) {
	int k = i - 1;
	for (int k = i - 1; k < i - 1 + 3; k++) {
		if (k >= 0 && k < height_)
			for (int l = j - 1; l < j - 1 + 3; l++) {
				if (l >= 0 && l < width_ && !(k == i && l == j)) {
					if (map_[k][l] == 0)
						map_[k][l] = value;
				}
			}
	}

}

double** brushfireMap::map() {
	return map_;
}

int brushfireMap::width() {
	return width_;
}

int brushfireMap::height() {
	return height_;
}

brushfireMap::~brushfireMap() {
	for (int i = 0; i < height_; i++)
		delete map_[i];

	delete[] map_;
}

/*
 for(int i=iters;(i<height_-iters) && (i>=0);i++)
 {
 for(int j=i;(j<width_-iters) && (j>=0);j++)
 {
 //printf("Map[%d][%d]=%d\n",i,j,map_[i][j]);
 if(map_[i][j] == 0)
 free_cell=true;
 else updateCellsAround(map_[i][j]+1,i,j);
 }
 }
 */
