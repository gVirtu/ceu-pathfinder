#define WORLD_MAX_W 100
#define WORLD_MAX_H 100

int world[WORLD_MAX_W * WORLD_MAX_H];

int world_w, world_h;

typedef struct {
    int x;
    int y;
} PathNode;

void loadWorld(char* path) {
    printf("Loading level from file %s...\n", path); 
    FILE* file = fopen(path, "r");
    //printf("Opened file...\n"); 
    char line[WORLD_MAX_W];

    int x, y, buf=0;
    while (fgets(line, sizeof(line), file)) {
        //printf("Read %s...\n", line); 
        for(x=0;line[x]!=0 && line[x]!='\n';++x) {
            world[buf++] = (line[x] == '#');
        }
        ++y;
        //printf("Stopped at %d\n", x);
    }
    world_w = x;
    world_h = y;
    //printf("Got world %dx%d\n", world_w, world_h);
    //printf("Closed file...\n"); 
    fclose(file);
}

int clamp(int x, int xmin, int xmax) {
    if (x>xmax) return xmax;
    if (x<xmin) return xmin;
    return x;
}

int sign(int x) {
    if (x<0) return -1;
    return 1;
}

int WorldAt(int x, int y) {
    if (x >= 0 && x < world_w && y >= 0 && y < world_h) {
        return world[y*world_w+x];
    } else {
        return -1;
    }
}

void PathNodeNeighbors(ASNeighborList neighbors, void *node, void *context) {
    PathNode *pathNode = (PathNode *)node;

    if (WorldAt(pathNode->x+1, pathNode->y) == 0) {
        ASNeighborListAdd(neighbors, &(PathNode){pathNode->x+1, pathNode->y}, 1);
    }
    if (WorldAt(pathNode->x-1, pathNode->y) == 0) {
        ASNeighborListAdd(neighbors, &(PathNode){pathNode->x-1, pathNode->y}, 1);
    }
    if (WorldAt(pathNode->x, pathNode->y+1) == 0) {
        ASNeighborListAdd(neighbors, &(PathNode){pathNode->x, pathNode->y+1}, 1);
    }
    if (WorldAt(pathNode->x, pathNode->y-1) == 0) {
        ASNeighborListAdd(neighbors, &(PathNode){pathNode->x, pathNode->y-1}, 1);
    }
}

float PathNodeHeuristic(void *fromNode, void *toNode, void *context) {
    PathNode *from = (PathNode *)fromNode;
    PathNode *to = (PathNode *)toNode;

    // using the manhattan distance since this is a simple grid and you can only move in 4 directions
    return (fabs(from->x - to->x) + fabs(from->y - to->y));
}

const ASPathNodeSource PathNodeSource = {
    sizeof(PathNode),
    &PathNodeNeighbors,
    &PathNodeHeuristic,
    NULL,
    NULL
};