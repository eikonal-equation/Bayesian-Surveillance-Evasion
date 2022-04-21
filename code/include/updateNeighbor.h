/**
 * @file updateNeighbor.h
 * @author Dongping Qi ()
 * @brief 
 *      - Local upwind scheme for Fast Marching Method
 *      - A Dijkstra-like graph node update function
 * @date 2022-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */

double upwindScheme(double h, double u_node, double uh, double uv, double F_node)
{
    double q = h / F_node;
    double vnew = u_node;

    double usum = uh + uv, udif = uh - uv;

    if (fabs(udif) <= q) {
        vnew = min(vnew, 0.5*(usum + sqrt(2 * q * q - udif * udif)));
    } else {
        vnew = min(vnew, min(uh, uv) + q);
    }
    return vnew;
}

void updateNeighbor(int node, int N, double h, double *u, int *status, MinHeap *minHeap, double F_node){
    if (status[node] != 2) {
        double uh = min(u[node - 1], u[node + 1]);
        double uv = min(u[node - N], u[node + N]);
        u[node] = upwindScheme(h, u[node], uh, uv, F_node);

        if (status[node] == 0) {
            minHeap -> array[minHeap -> hsize] = newMinHeapNode(node, DBL_MAX);
            minHeap -> pos[node] = minHeap -> hsize;
            minHeap -> hsize = minHeap -> hsize + 1;
            
            decreaseKey(minHeap, node, u[node]);
            status[node] = 1;
        } else {
            decreaseKey(minHeap, node, u[node]);
        }
    }
}