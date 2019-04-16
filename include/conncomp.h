#include <iostream>
#include <fstream>
#include <map>

using namespace std;

inline int getHash(Point* p, Point* maxp)
{
	return (p->x*maxp->y*maxp->z + p->y*maxp->z + p->z);
}

void build_voxelmap(Point* voxels, int nvoxels, Point* volsize, map<int, bool>& voxelmap)
{
	voxelmap.clear();
	Point* p_voxel = voxels;
	for (int i = 0; i < nvoxels; ++i, ++p_voxel) {
		voxelmap[getHash(voxels+i, volsize)] = true;
	}
}

void filter_connected_components(Point* voxels, int nvoxels, Point* volsize, map<int, bool>& voxelmap, int threshold, vector<Point*> &myobj)
{
	int ncomp = 0;
	Point* p_voxel = voxels;
	Point voxel_next;
	int hv;
	int dx, dy, dz, x, y, z, x_next, y_next, z_next;
	Point* voxel_stack = new Point[nvoxels];
	int compsize, p_stack;
	int n_out_voxels = 0;

	myobj.clear();
	for (int i = 0; i < nvoxels; ++i, ++p_voxel) {
		hv = getHash(p_voxel, volsize);
		if (voxelmap.find(hv) != voxelmap.end() && voxelmap[hv] == true) {
			//	start a new component, bfs
			compsize = 1; p_stack = 0;
			voxel_stack[0] = *p_voxel;
			voxelmap[hv] = false;

			while (p_stack < compsize) {
				x = voxel_stack[p_stack].x;
				y = voxel_stack[p_stack].y;
				z = voxel_stack[p_stack].z;
				++p_stack;

				for (dx = -1; dx <= 1; ++dx) {
					x_next = x + dx;
					if (x_next < 0 && x_next >= volsize.x) continue;
					for (dy = -1; dy <= 1; ++dy) {
						y_next = y + dy;
						if (y_next < 0 && y_next >= volsize.y) continue;
						for (dz = -1; dz <=1; ++dz) {
							z_next = z + dz;
							if (z_next < 0 && z_next >= volsize.z) continue;
							voxel_next = Point(x_next, y_next, z_next);
							hv = getHash(&voxel_next, volsize);
							if (voxelmap.find(hv) != voxelmap.end() && voxelmap[hv] == true) {
								voxelmap[hv] = false;
								voxel_stack[compsize++] = voxel_next;
							}
						}
					}
				}
			}
			++ncomp;
			cout << compsize << " ";

			if (compsize > threshold) {
				n_out_voxels += compsize;
				for (p_stack = 0; p_stack < compsize; ++p_stack)
					ofs << voxel_stack[p_stack].x << " " 
					<< voxel_stack[p_stack].y << " "
					<< voxel_stack[p_stack].z << endl;
			}

		}
	}
	ofs.close();
	cout << endl;
	cout << ncomp << " components in total." << endl;
	cout << n_out_voxels << " voxels in total." << endl;
}

void refine()
{
	//	build a map for voxels
	map<int, bool> voxelmap;
	build_voxelmap(voxels, nvoxels, volsize, voxelmap);
	//	output
	output_large_connected_components(voxels, nvoxels, volsize, voxelmap, 1000, "voxel_final.out");
	return 0;
}
