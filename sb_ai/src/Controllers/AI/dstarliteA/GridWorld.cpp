#include "GridWorld.h"

/*
BEFORE ATTEMPTING TO READ THIS FILE, PLEASE HAVE A BASIC UNDERSTANDING OF D*LITE
FROM READING ITS ORIGINAL RESEARCH PAPER'S PSEUDO-CODE.
*/

GridWorld::GridWorld(unsigned int length, unsigned int width, int radius, Coords startCoords, Coords goalCoords){
	this->length = length;
	this->width = width;
	this->radius = radius;
	for (unsigned int y = 0; y < length; y++){
		for (unsigned int x = 0; x < width; x++){
			world.push_back(new Tile(x, y, 10));
		}
	}

	start = getTileAt(startCoords.first, startCoords.second);
	goal = getTileAt(goalCoords.first, goalCoords.second);

	//Initializing the pathfinder's default values
	km = 0;
	previous = start;
	goal->rhs = 0;

	goal->isOpen = true;
	goal->h = calculateH(goal);
	goal->key = GridWorld::KeyPair(goal->h, 0);
	open.push_back(goal);
}

std::vector<GridWorld::Coords> GridWorld::getTraversal(){
	std::vector<GridWorld::Coords> resultantPath;

	while (start != goal){
		if (start->rhs == PF_INFINITY){
			std::cout << "\tNO PATH EXIST" << std::endl;
			break;
		}

		start = getMinSuccessor(start).first;
		if (start != 0){
			std::cout << "\tMoved to: (" << start->x << ", " << start->y << ")" << std::endl;
			resultantPath.push_back(Coords(start->x, start->y));
		}
		else{
			std::cout << "\tNULL SUCCESSOR, unable to find the next tile" << std::endl;
		}
	}
	return resultantPath;
}

/*
This method handles the inflation of surrounding tiles. When an obsticle
is detected, the surrounding tile's cost can be inflated to make the robot
avoid getting anywhere near the obsticle. By the nature of pathfinding on
grid maps, the resultant path tends to "hug the wall", however this will make
the path have some distance between itself and any obsticles
*/
void GridWorld::inflate(unsigned int x, unsigned int y, double newCost){
	updateCost(x, y, newCost);
	for (int dy = -radius; dy <= radius; dy++){
		for (int dx = -radius; dx <= radius; dx++){
			if (abs(dy) + abs(dx) != 0){
				Tile* t = getTileAt(x + dx, y + dy);
				if (t != 0 && newCost > t->cost){
					updateCost(x + dx, y + dy, INFLATION);
				}

			}
		}
	}
}

/*
This method does the same thing as the pseudo-code's updateVertex(),
except for grids instead of graphs.

Pathfinding algorimths tend to be demonstraited with a graph rather than a grid,
in order to update the cost between two tiles we must update both the tile and its neighbour.
*/
void GridWorld::updateCost(unsigned int x, unsigned int y, double newCost){
	static int count = 1;
	count++;
	Tile* tile = getTileAt(x, y);

	printf("Updating <%d, %d> from %2.0lf to %2.0lf - Update: %d\n", x, y, tile->cost, newCost, count);
	km += calculateH(previous);
	previous = start;

	//I am aware that the following code below could be refactored by 50%
	//since it's repeating itself with only a few changes

	double oldCost = tile->cost;
	double oldCostToTile, newCostToTile;
	double currentRHS, otherG;

	//Update CURRENT by finding its new minimum RHS-value from NEIGHBOURS
	std::vector<Tile*> neighbours(getNeighbours(tile));
	for (int i = 0; i < neighbours.size(); i++){
		tile->cost = oldCost;
		oldCostToTile = calculateC(tile, neighbours[i]);

		tile->cost = newCost;
		newCostToTile = calculateC(tile, neighbours[i]);

		currentRHS = tile->rhs;
		otherG = neighbours[i]->g;

		if (oldCostToTile > newCostToTile){
			if (tile != goal){
				tile->rhs = std::min(currentRHS, (newCostToTile + otherG));
			}
		}
		else if (currentRHS == (oldCostToTile + otherG)){
			if (tile != goal){
				tile->rhs = getMinSuccessor(tile).second;
			}
		}
	}

	updateVertex(tile);

	//Update all NEIGHBOURING cells by finding their new min RHS-values from CURRENT
	for (int i = 0; i < neighbours.size(); i++){
		tile->cost = oldCost;
		oldCostToTile = calculateC(tile, neighbours[i]);

		tile->cost = newCost;
		newCostToTile = calculateC(tile, neighbours[i]);

		currentRHS = neighbours[i]->rhs;
		otherG = tile->g;

		if (oldCostToTile > newCostToTile){
			if (neighbours[i] != goal){
				neighbours[i]->rhs = std::min(currentRHS, (newCostToTile + otherG));
			}
		}
		else if (currentRHS == (oldCostToTile + otherG)){
			if (neighbours[i] != goal){
				neighbours[i]->rhs = getMinSuccessor(neighbours[i]).second;
			}
		}
		updateVertex(neighbours[i]);
	}

	computeShortestPath();
}

bool GridWorld::computeShortestPath(){
	if (open.empty()){
		std::cout << "ERROR: No tiles to expand on... can't do anything" << std::endl;
		return false;
	}

	double currentRHS, otherG, previousG;

	while (!open.empty() && (compareKeys(open.front()->key, start->key = calculateKey(start)) || start->rhs != start->g)){
		Tile* current = open.front();
		//Notice that CURRENT wasn't pop/removed yet..

		KeyPair k_old = current->key;
		KeyPair k_new = calculateKey(current);

		currentRHS = current->rhs;
		otherG = current->g;

		/*std::cout << "Expanding:";
		current->info();
		std::cout << std::endl;*/

		if (compareKeys(k_old, k_new)){
			//This branch updates tile that were already in the OPEN list originally
			//This branch tends to execute AFTER the else branch
			current->key = k_new;
			make_heap(open.begin(), open.end(), GridWorld::compareTiles);

		}
		else if (otherG > currentRHS){
			//Majority of the execution will fall under this conditional branch as
			//it is undergoing normal A* pathfinding

			current->g = current->rhs;
			otherG = currentRHS;

			open.erase(open.begin());
			make_heap(open.begin(), open.end(), GridWorld::compareTiles);
			current->isOpen = false;

			std::vector<Tile*> neighbours(getNeighbours(current));
			for (int i = 0; i < neighbours.size(); i++){
				if (neighbours[i] != 0){
					if (neighbours[i] != goal){
						neighbours[i]->rhs = std::min(neighbours[i]->rhs, calculateC(current, neighbours[i]) + otherG);
					}
					updateVertex(neighbours[i]);
				}
			}

		}
		else {
			//Execution of this branch updates the tile during incremental search

			previousG = otherG;
			current->g = PF_INFINITY;

			//Update CURRENT'S RHS
			if (current != goal){
				current->rhs = getMinSuccessor(current).second;
			}
			updateVertex(current);

			//Update NEIGHBOUR'S RHS to their minimum successor
			std::vector<Tile*> neighbours(getNeighbours(current));
			for (int i = 0; i < neighbours.size(); i++){
				if (neighbours[i] != 0){
					if (neighbours[i]->rhs == (calculateC(current, neighbours[i]) + previousG) && neighbours[i] != goal){
						neighbours[i]->rhs = getMinSuccessor(neighbours[i]).second;
					}
					updateVertex(neighbours[i]);
				}
			}

		}
		//Uncomment this to see CURRENT'S new values
		//std::cout << "Expanded:";
		//current->info();
		//std::cout << std::endl;
	}
	return true;
}

void GridWorld::updateVertex(GridWorld::Tile*& tile){
	bool isIncosistent = tile->rhs != tile->g;

	if (isIncosistent && tile->isOpen){
		tile->key = calculateKey(tile);
		make_heap(open.begin(), open.end(), GridWorld::compareTiles);

	}
	else if (isIncosistent && !tile->isOpen){
		tile->key = calculateKey(tile);
		tile->isOpen = true;

		open.push_back(tile);
		push_heap(open.begin(), open.end(), GridWorld::compareTiles);

	}
	else if (!isIncosistent && tile->isOpen){
		open.erase(std::find(open.begin(), open.end(), tile));
		make_heap(open.begin(), open.end(), GridWorld::compareTiles);
		tile->isOpen = false;
	}

}


bool GridWorld::withinWorld(unsigned int x, unsigned int y) const{
	return y >= 0 && y < length && x >= 0 && x < width;
}

GridWorld::Tile* GridWorld::getTileAt(unsigned int x, unsigned int y) const{
	return withinWorld(x, y) ? (world.at(y*width + x)) : NULL;
}

std::vector<GridWorld::Tile*> GridWorld::getNeighbours(Tile*& tile){
	std::vector<GridWorld::Tile*> neighbours;
	for (int dy = -1; dy <= 1; dy++){
		for (int dx = -1; dx <= 1; dx++){
			if (abs(dy) + abs(dx) != 0){
				Tile* neighbour = getTileAt(tile->x + dx, tile->y + dy);
				if (neighbour != 0){
					neighbours.push_back(neighbour);
				}
			}
		}
	}
	return neighbours;
}

bool GridWorld::compareTiles(Tile*& left, Tile*& right){
	/*
	The heap functions from <algorithm> operates as a MAX heap,
	by making sure that left < right. In order to operate as a MIN heap
	we'll reverse the order by comparing right < left
	*/
	return compareKeys(right->key, left->key);
}

bool GridWorld::compareKeys(const KeyPair& left, const KeyPair& right){
	if (left.first < right.first){
		return true;
	}
	else if (left.first == right.first && left.second < right.second){
		return true;
	}
	else {
		return false;
	}
}

GridWorld::TilePair GridWorld::getMinSuccessor(GridWorld::Tile*& tile){
	Tile* minTile = 0;
	double minCost = PF_INFINITY;

	std::vector<Tile*> neighbours(getNeighbours(tile));
	for (int i = 0; i < neighbours.size(); i++){
		double cost = calculateC(tile, neighbours[i]);
		double g = neighbours[i]->g;

		if (cost == PF_INFINITY || g == PF_INFINITY){
			continue;
		}

		if (cost + g < minCost){   //potential overflow?
			minTile = neighbours[i];
			minCost = cost + g;
		}
	}

	return TilePair(minTile, minCost);
}

double GridWorld::calculateH(GridWorld::Tile*& tile){
	unsigned int dx = labs(tile->x - start->x);
	unsigned int dy = labs(tile->y - start->y);

	if (dx > dy){
		std::swap(dx, dy);
	}

	return ((SQRT2 - 1) * dx + dy) * 10;
}

double GridWorld::calculateC(GridWorld::Tile*& tileA, GridWorld::Tile*& tileB){
	if (tileA->cost == PF_INFINITY || tileB->cost == PF_INFINITY){
		return PF_INFINITY;
	}

	if (labs(tileA->x - tileB->x) + labs(tileA->y - tileB->y) == 2){
		//These two tiles are diagonally adjacent to each other
		return SQRT2 * (tileA->cost + tileB->cost) / 2;
	}
	return (tileA->cost + tileB->cost) / 2;
}

GridWorld::KeyPair GridWorld::calculateKey(GridWorld::Tile*& tile){
	double key2 = std::min(tile->g, tile->rhs);
	double key1 = key2 + calculateH(tile) + km;
	//H-value should be re-calculated every call as it can change during incremental search

	return KeyPair(key1, key2);
}

GridWorld::Tile::Tile(unsigned int x, unsigned int y, double cost) : x(x), y(y){
	this->cost = cost;

	this->rhs = PF_INFINITY;
	this->g = PF_INFINITY;
	this->h = 0;

	this->isOpen = false;
}

GridWorld::Tile::Tile(Tile& other) : x(other.x), y(other.y){
	this->cost = other.cost;

	this->rhs = other.rhs;
	this->g = other.g;
	this->h = other.h;

	this->isOpen = other.isOpen;
}

void GridWorld::Tile::info() const{

	printf("[(%d, %d)  H: %.2lf, G: %.2lf, RHS: %.2lf, K:<%.2lf, %.2lf>]", this->x, this->y,
		this->h == PF_INFINITY ? -1 : this->h,
		this->g == PF_INFINITY ? -1 : this->g,
		this->rhs == PF_INFINITY ? -1 : this->rhs,
		this->key.first == PF_INFINITY ? -1 : this->key.first,
		this->key.second == PF_INFINITY ? -1 : this->key.second);
}

void GridWorld::printWorld() const{
	std::cout << "H:" << std::endl;
	for (unsigned int y = 0; y < length; y++){
		for (unsigned int x = 0; x < width; x++){
			printf("%2.0lf ", getTileAt(x, y)->h);
		}
		std::cout << std::endl;
	}

	std::cout << "C:" << std::endl;
	for (unsigned int y = 0; y < length; y++){
		for (unsigned int x = 0; x < width; x++){
			double cost = getTileAt(x, y)->cost;
			if (cost == PF_INFINITY){
				printf("-1 ");
			}
			else if (cost == INFLATION){
				printf("^^ ");
			}
			else{
				printf("%2.0lf ", cost);
			}

		}
		std::cout << std::endl;
	}

	std::cout << "G:" << std::endl;
	for (unsigned int y = 0; y < length; y++){
		for (unsigned int x = 0; x < width; x++){
			printf("%2.0lf ", getTileAt(x, y)->g == PF_INFINITY ? -1 : getTileAt(x, y)->g);
		}
		std::cout << std::endl;
	}

	std::cout << "RHS:" << std::endl;
	for (unsigned int y = 0; y < length; y++){
		for (unsigned int x = 0; x < width; x++){
			printf("%2.0lf ", getTileAt(x, y)->rhs == PF_INFINITY ? -1 : getTileAt(x, y)->rhs);
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
