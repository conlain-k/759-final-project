// Uses mesh gotten from here: https://graphics.stanford.edu/~mdfisher/Data/Meshes/bunny.obj
// Uses sample code from the tinyobj loader example
// Load an obj, print some data
// Usage: ./tester data/bunny.obj works rn

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "detect.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader/tiny_obj_loader.h"
using std::string;
using tinyobj::real_t;

// A single point in 3D space
typedef real_t Point[3];

static void PrintInfo(const tinyobj::attrib_t& attrib,
					  const std::vector<tinyobj::shape_t>& shapes,
					  const std::vector<tinyobj::material_t>& materials) {
	std::cout << "# of vertices  : " << (attrib.vertices.size() / 3) << std::endl;
	std::cout << "# of normals   : " << (attrib.normals.size() / 3) << std::endl;
	std::cout << "# of texcoords : " << (attrib.texcoords.size() / 2) << std::endl;

	std::cout << "# of shapes    : " << shapes.size() << std::endl;
	std::cout << "# of materials : " << materials.size() << std::endl;

	for (size_t v = 0; v < attrib.vertices.size() / 3; v++) {
		printf("  v[%ld] = (%f, %f, %f)\n", static_cast<long>(v),
			   static_cast<const double>(attrib.vertices[3 * v + 0]),
			   static_cast<const double>(attrib.vertices[3 * v + 1]),
			   static_cast<const double>(attrib.vertices[3 * v + 2]));
	}

	for (size_t v = 0; v < attrib.normals.size() / 3; v++) {
		printf("  n[%ld] = (%f, %f, %f)\n", static_cast<long>(v),
			   static_cast<const double>(attrib.normals[3 * v + 0]),
			   static_cast<const double>(attrib.normals[3 * v + 1]),
			   static_cast<const double>(attrib.normals[3 * v + 2]));
	}

	for (size_t v = 0; v < attrib.texcoords.size() / 2; v++) {
		printf("  uv[%ld] = (%f, %f)\n", static_cast<long>(v),
			   static_cast<const double>(attrib.texcoords[2 * v + 0]),
			   static_cast<const double>(attrib.texcoords[2 * v + 1]));
	}

	// For each shape
	for (size_t i = 0; i < shapes.size(); i++) {
		printf("shape[%ld].name = %s\n", static_cast<long>(i), shapes[i].name.c_str());
		printf("Size of shape[%ld].indices: %lu\n", static_cast<long>(i),
			   static_cast<unsigned long>(shapes[i].mesh.indices.size()));

		size_t index_offset = 0;

		assert(shapes[i].mesh.num_face_vertices.size() == shapes[i].mesh.material_ids.size());

		printf("shape[%ld].num_faces: %lu\n", static_cast<long>(i),
			   static_cast<unsigned long>(shapes[i].mesh.num_face_vertices.size()));

		// For each face
		for (size_t f = 0; f < shapes[i].mesh.num_face_vertices.size(); f++) {
			size_t fnum = shapes[i].mesh.num_face_vertices[f];

			printf("  face[%ld].fnum = %ld\n", static_cast<long>(f), static_cast<unsigned long>(fnum));

			// For each vertex in the face
			for (size_t v = 0; v < fnum; v++) {
				tinyobj::index_t idx = shapes[i].mesh.indices[index_offset + v];
				printf("    face[%ld].v[%ld].idx = %d/%d/%d\n", static_cast<long>(f), static_cast<long>(v),
					   idx.vertex_index, idx.normal_index, idx.texcoord_index);
			}

			printf("  face[%ld].material_id = %d\n", static_cast<long>(f), shapes[i].mesh.material_ids[f]);

			index_offset += fnum;
		}

		printf("shape[%ld].num_tags: %lu\n", static_cast<long>(i),
			   static_cast<unsigned long>(shapes[i].mesh.tags.size()));
		for (size_t t = 0; t < shapes[i].mesh.tags.size(); t++) {
			printf("  tag[%ld] = %s ", static_cast<long>(t), shapes[i].mesh.tags[t].name.c_str());
			printf(" ints: [");
			for (size_t j = 0; j < shapes[i].mesh.tags[t].intValues.size(); ++j) {
				printf("%ld", static_cast<long>(shapes[i].mesh.tags[t].intValues[j]));
				if (j < (shapes[i].mesh.tags[t].intValues.size() - 1)) {
					printf(", ");
				}
			}
			printf("]");

			printf(" floats: [");
			for (size_t j = 0; j < shapes[i].mesh.tags[t].floatValues.size(); ++j) {
				printf("%f", static_cast<const double>(shapes[i].mesh.tags[t].floatValues[j]));
				if (j < (shapes[i].mesh.tags[t].floatValues.size() - 1)) {
					printf(", ");
				}
			}
			printf("]");

			printf(" strings: [");
			for (size_t j = 0; j < shapes[i].mesh.tags[t].stringValues.size(); ++j) {
				printf("%s", shapes[i].mesh.tags[t].stringValues[j].c_str());
				if (j < (shapes[i].mesh.tags[t].stringValues.size() - 1)) {
					printf(", ");
				}
			}
			printf("]");
			printf("\n");
		}
	}
}
int main(int argc, char const* argv[]) {
	if (argc < 3) {
		std::cerr << "Usage: ./solver <filename1> <filename2>" << std::endl;
		exit(1);
	}
	string filename(argv[1]);
	string filename2(argv[2]);
	tinyobj::attrib_t attrib, attrib2;
	std::vector<tinyobj::shape_t> shapes, shapes2;
	std::vector<tinyobj::material_t> materials, materials2;
	std::string err;
	bool triangulate = true;
	bool ret;
	ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str(), NULL, triangulate);

	if (!err.empty()) {
		std::cerr << err << std::endl;
	}
	if (!ret) {
		std::cerr << "couldn't load first bunny!" << std::endl;
	}
	ret = tinyobj::LoadObj(&attrib2, &shapes2, &materials2, &err, filename2.c_str(), "data/", triangulate);
	if (!err.empty()) {
		std::cerr << err << std::endl;
	}
	if (!ret) {
		std::cerr << "couldn't load first bunny!" << std::endl;
	}
	// for ()
	// int tri_tri_intersect(float V0[3],float V1[3],float V2[3],
	//                       float U0[3],float U1[3],float U2[3])
	// PrintInfo(attrib, shapes, materials);
	float V0[] = {0, 0, 0};
	float V1[] = {1, 0, 0};
	float V2[] = {0, 1, 0};
	float U0[] = {0, 0, 0};
	float U1[] = {0, 1, 0};
	float U2[] = {0, 1, 1};
	std::cout << tri_tri_intersect(V0, V1, V2, U0, U1, U2) << std::endl;
	float U3[] = {2, 3, 2};
	float U4[] = {2, 2, 2};
	float U5[] = {3, 2, 2};
	std::cout << tri_tri_intersect(V0, V1, V2, U3, U4, U5) << std::endl;
	// This is a messy c-style cast
	Point* mesh1Points = (Point*)attrib.vertices.data();
	Point* mesh2Points = (Point*)attrib2.vertices.data();
	// for (unsigned int i = 0; i < attrib.vertices.size() / 3; i++) {
	// 	printf("point is %f, %f, %f\n", mesh1Points[i][0], mesh1Points[i][1], mesh1Points[i][2]);
	// 	printf("  v[%ld] = (%f, %f, %f)\n", static_cast<long>(i),
	// 		   static_cast<const double>(attrib.vertices.data()[3 * i + 0]),
	// 		   static_cast<const double>(attrib.vertices.data()[3 * i + 1]),
	// 		   static_cast<const double>(attrib.vertices.data()[3 * i + 2]));
	// }

	// For each shape
	for (size_t i = 0; i < shapes.size(); i++) {
		// For each face
		for (size_t f = 0; f < shapes[i].mesh.num_face_vertices.size(); f++) {
			size_t fnum = shapes[i].mesh.num_face_vertices[f];
			size_t index_offset = 0;
			printf("face %ld\n", static_cast<long>(f));
			// For each vertex in the face
			Point* tri1 = new Point[3];
			for (int p = 0; p < 3; p++) {
				tinyobj::index_t idx = shapes[i].mesh.indices[index_offset + p];

			}
			for (int v = 0; v < 3; v++) {
				tinyobj::index_t idx = shapes[i].mesh.indices[index_offset + v];
				mesh1Points[idx.vertex_index];
				tri1[i][v] = mesh1Points[idx.vertex_index][0];
				tri1[i][v] = mesh1Points[idx.vertex_index][1];
				tri1[i][v] = mesh1Points[idx.vertex_index][2];
				index_offset += fnum;
			}
			printf("point is %f, %f, %f\n", tri1[i][0], tri1[i][1], tri1[i][2]);
			printf("point is %f, %f, %f\n", mesh1Points[idx.vertex_index][0],
				   mesh1Points[idx.vertex_index][1], mesh1Points[idx.vertex_index][2]);
			std::cout << "start inner loop" << std::endl;
			for (size_t j = 0; j < shapes.size(); j++) {
				// For each face
				for (size_t face2 = 0; face2 < shapes2[j].mesh.num_face_vertices.size(); face2++) {
					size_t index_offset2 = 0;
					size_t fnum2 = shapes2[i].mesh.num_face_vertices[f];

					// printf("face %ld\n", static_cast<long>(face2));
					// For each vertex in the face
					Point tri2[3];
					for (size_t v = 0; v < 3; v++) {
						tinyobj::index_t idx = shapes2[i].mesh.indices[index_offset2 + v];
						// tri2[v] = mesh2Points[idx.vertex_index];
						index_offset2 += fnum2;
					}
					if (tri_tri_intersect(tri1[0], tri1[1], tri1[1], tri2[0], tri2[1], tri2[2])) {
						std::cout << "face " << f << " on body 1 collides with face " << face2 << " on body 2"
								  << std::endl;
					}
					std::cout << "body 1:" << std::endl;
					for (size_t v = 0; v < 3; v++) {
						printf("point %d is %f, %f, %f\n", v, mesh1Points[v][0], mesh1Points[v][1],
							   mesh1Points[v][2]);
					}

					printf("point is %f, %f, %f\n", mesh1Points[i][0], mesh1Points[i][1], mesh1Points[i][2]);
				}
			}
			delete[] tri1;
		}
	}
	printf("size of point is %lu\n", sizeof(Point));
	return 0;
}
