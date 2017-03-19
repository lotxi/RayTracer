#include "Scene.h"
#include "InputReader.h"
#pragma warning(disable:4996)
#include "Sphere.h"
#include "Plane.h"

void saveBMP(const char * fileName, int w, int h, int dpi, glm::vec3 *data)
{
	FILE *f;
	int k = w*h;
	int s = 4 * k;
	int filesize = 54 + s;

	double factor = 39.375;
	int m = static_cast<int>(factor);

	int ppm = dpi*m;
	unsigned char bmpfileheader[14] = { 'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0 };
	unsigned char bmpinfoheader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0 };

	bmpfileheader[2] = (unsigned char)(filesize);
	bmpfileheader[3] = (unsigned char)(filesize >> 8);
	bmpfileheader[4] = (unsigned char)(filesize >> 16);
	bmpfileheader[5] = (unsigned char)(filesize >> 24);

	bmpinfoheader[4] = (unsigned char)(w);
	bmpinfoheader[5] = (unsigned char)(w >> 8);
	bmpinfoheader[6] = (unsigned char)(w >> 16);
	bmpinfoheader[7] = (unsigned char)(w >> 24);

	bmpinfoheader[8] = (unsigned char)(h);
	bmpinfoheader[9] = (unsigned char)(h >> 8);
	bmpinfoheader[10] = (unsigned char)(h >> 16);
	bmpinfoheader[11] = (unsigned char)(h >> 24);

	bmpinfoheader[21] = (unsigned char)(s);
	bmpinfoheader[22] = (unsigned char)(s >> 8);
	bmpinfoheader[23] = (unsigned char)(s >> 16);
	bmpinfoheader[24] = (unsigned char)(s >> 24);

	bmpinfoheader[25] = (unsigned char)(ppm);
	bmpinfoheader[26] = (unsigned char)(ppm >> 8);
	bmpinfoheader[27] = (unsigned char)(ppm >> 16);
	bmpinfoheader[28] = (unsigned char)(ppm >> 24);


	bmpinfoheader[29] = (unsigned char)(ppm);
	bmpinfoheader[30] = (unsigned char)(ppm >> 8);
	bmpinfoheader[31] = (unsigned char)(ppm >> 16);
	bmpinfoheader[32] = (unsigned char)(ppm >> 24);

	f = fopen(fileName, "wb");
	fwrite(bmpfileheader, 1, 14, f);
	fwrite(bmpinfoheader, 1, 40, f);

	for (int i = 0; i<k; i++)
	{
		double red = data[i].x * 255;
		double green = data[i].y * 255;
		double blue = data[i].z * 255;
		unsigned char color[3] = { (int)floor(blue),(int)floor(green),(int)floor(red) };
		fwrite(color, 1, 3, f);
	}
	fclose(f);
}

int main()
{
	int dpi = 72;
	int width = 640;
	int height = 480;
	int numPixels = width * height;
	glm::vec3* pixels = new glm::vec3[numPixels];

	glm::vec3 X = glm::vec3(1, 0, 0);
	glm::vec3 Y = glm::vec3(0, 1, 0);
	glm::vec3 Z = glm::vec3(0, 0, 1);

	glm::vec3 cam_dir = glm::vec3(0, 0, -1);
	glm::vec3 cam_down = glm::vec3(0, -1, 0);
	glm::vec3 cam_right = glm::vec3(0, 1, 0);

	//Sphere scene_sphere(glm::vec3(0, 10, -30), glm::vec3(0.1, 0.5, 0.5), glm::vec3(0.4, 0.6, 0.2), glm::vec3(0.2, 0.5, 0.5), 3, 1.0);
	//Plane scene_plane(Y, glm::vec3(), glm::vec3(0.2, 0.2, 0.2), glm::vec3(0.8, 0.8, 0.2), glm::vec3(0.5, 0.5, 0.5),1.0);
	//Camera scene_camera()

	InputReader* input = new InputReader("scene5.txt");
	Scene scene = input->scene;
	//delete input;
	float aspect_ratio = scene.camera->getAspectRatio();
	glm::vec3 Cam_ray_origin = scene.camera->getPosition();


	float xamt, yamt;
	int index = 0;

	for (int x=0 ; x< width; x++)
	{
		for (int y=0; y<height; y++)
		{
			//No anti-aliasing
			if (width > height)
			{
				xamt = ((x + 0.5) / width)*aspect_ratio- (((width - height) / (float)height) / 2);
				yamt = ((height - y) + 0.5)/height;
			}
			else if (height >width)
			{
				xamt = (x + 0.5) / width;
				yamt = (((height - y) + 0.5) / height) / aspect_ratio - (((height - width) / (float)width) / 2);
			}
			else
			{
				// The image is square
				xamt = (x + 0.5) / width;
				yamt = ((height - y) + 0.5) / height;
			}
			glm::vec3 Cam_ray_direction = cam_dir + (cam_right*(float)(xamt - 0.5) + (cam_down*(float)(yamt - 0.5)));
			Ray cam_ray(Cam_ray_origin, Cam_ray_direction);

			std::vector<float> intersections;

			

			
			index = y*width + x;
			if ((x>200 && x< 440) && (y>200 && y<280))
			{
				pixels[index].x = 0.5;
				pixels[index].y = 0.1;
				pixels[index].z = 1.0;
			}
			else
			{
				pixels[index].x = 0.1;
				pixels[index].y = 0.1;
				pixels[index].z = 0.2;
			}
		}
		
	}
	saveBMP("scene.bmp", width, height, dpi, pixels);

	/*InputReader* input = new InputReader("scene5.txt");
	Scene scene = input->scene;*/
	//std::cout << scene.to_string() << std::endl;
	return 0;
} 