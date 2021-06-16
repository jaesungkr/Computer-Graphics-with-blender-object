// UNNC 20121762 JAESUNG PARK. Last modified. 2020/12/21

#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;

// Object created for importing objects from Blender
class CPoint2f{
public:
	vector <float> d{ 0, 0 };
};

class CPoint3f{
public:
	vector <float> d{ (0, 0, 0) };
};


class CPoint3i{
public:
	vector <int> d{ (0, 0, 0) };
};

class CFace {
public:
	vector <CPoint3i> v_pairs;
};

class CObj{
public:
	string name;
	vector <CPoint3f> v;
	vector <CPoint2f> vt;
	vector <CPoint3f> vn;
	vector <CFace> f;
};

class CModel{
public:
	vector <CObj> objs;
	vector<float> my_strtok_f(char* str, char*delimeter){
		vector <float> v;
		char* context;
		char* tok = strtok_s(str, delimeter, &context);
		while (tok != NULL){
			v.push_back(atof(tok));
			tok = strtok_s(NULL, delimeter, &context);
		}
		return v;
	}

	vector<string> my_strtok_s(char* str, char*delimeter){
		vector <string> v;
		char* context;
		char* tok = strtok_s(str, delimeter, &context);
		while (tok != NULL){
			v.push_back(tok);
			tok = strtok_s(NULL, delimeter, &context);
		}
		return v;
	}
	vector<int> my_strtok_i(char* str, char*delimeter){
		vector <int> v;
		char* context;
		char* tok = strtok_s(str, delimeter, &context);
		while (tok != NULL){
			v.push_back(atof(tok));
			tok = strtok_s(NULL, delimeter, &context);
		}
		return v;
	}

	void loadObj(ifstream& fin){ 
		string line;
		CObj obj_tmp;
		int cnt = 0;

		int cnt_prev_vertex = 0;
		int cnt_prev_texture = 0;
		int cnt_prev_normal = 0;

		while (getline(fin, line)){
			int len = line.length();
			vector<float> vf;
			vector<string> s;
			vector<int> vi;
			CPoint3f p3;
			CPoint2f p2;
			CPoint3i p3i;
			if (line[0] == 'o' && line[1] == ' '){
				obj_tmp.name = line.substr(1, len - 2);
				objs.push_back(obj_tmp);
				if (cnt > 0){
					cnt_prev_vertex += objs[cnt - 1].v.size();
					cnt_prev_texture += objs[cnt - 1].vt.size();
					cnt_prev_normal += objs[cnt - 1].vn.size();
				}
				cnt += 1;
			}
			if (line[0] == 'v' && line[1] == ' '){
				vf = my_strtok_f((char*)line.substr(2, len - 2).c_str(), (char*)" ");
				p3.d = { vf[0], vf[1], vf[2] };
				objs[cnt - 1].v.push_back(p3);
			}
			else if (line[0] == 'v' && line[1] == 't'){
				vf = my_strtok_f((char*)line.substr(3, len - 3).c_str(), (char*)" ");
				p2.d = { vf[0], vf[1] };
				objs[cnt - 1].vt.push_back(p2);
			}

			else if (line[0] == 'v' && line[1] == 'n'){
				vf = my_strtok_f((char*)line.substr(3, len - 3).c_str(), (char*)" ");
				p3.d = { vf[0], vf[1], vf[2] };
				objs[cnt - 1].vn.push_back(p3);
			}
			else if (line[0] == 'f' && line[1] == ' '){
				s = my_strtok_s((char*)line.substr(2, len - 2).c_str(), (char*)" ");
				int nVertexes = s.size();
				CFace face_tmp;
				for (int i = 0; i < nVertexes; i++){
					vi = my_strtok_i((char*)s[i].c_str(), (char*)"/");
					p3i.d = { vi[0]-cnt_prev_vertex, vi[1]-cnt_prev_texture, vi[2]-cnt_prev_normal };
					face_tmp.v_pairs.push_back(p3i);
				}
				objs[cnt - 1].f.push_back(face_tmp);
			}
		}
	}


};
float ballAngle = 0;


GLfloat material_0_emission[] = { 0.0, 0.0, 1.0, 1.0 };
void InitLight(){

	

	GLfloat light_global_ambient[] = { 0.4, 0.4, 0.4, 1.0 };
	GLfloat light_0_pos[] = { 0.0, 3.0, 0.0, 1 };
	
	GLfloat light_0_ambient[] = { 0.1, 0.1, 0.5, 1.0 };
	GLfloat light_0_diffuse[] = { 1.0, 1.0, 0.5, 1.0 };
	GLfloat light_0_specular[] = { 0.2, 0.2, 0.2, 1.0 };

	GLfloat material_0_ambient[] = { 0.5, 0.0, 0.0, 1.0 };
	GLfloat material_0_diffuse[] = { 0.3, 0.0, 0.0, 1.0 };
	GLfloat material_0_specular[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat material_0_shininess[] = { 25.0 }; // 0~128
	
	
	GLfloat light_1_pos[] = { 0.0, 10.0, 0.0, 1 };
	GLfloat light_1_dir[] = { 0.0, -1.0, 0.0, 1 };
	GLfloat spotAngle[] = { 30.0 };
	GLfloat spotExp1[] = { 1.0 };
	
	

	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	
	glEnable(GL_LIGHT0); 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_global_ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, light_0_pos);
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, material_0_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_0_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_0_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material_0_shininess);
	
	
	glEnable(GL_COLOR_MATERIAL);
	
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);  //enhancement
}
void InitVisibility(){
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
}

#define _WINDOW_WIDTH 500
#define _WINDOW_HEIGHT 500

GLfloat camx = 0, camy = 0, camz = 5.0f;
GLfloat cam2x = 0, cam2y = 0, cam2z = -1.0;
GLfloat cam_upx = 0, cam_upy = 1, cam_upz = 0;
GLfloat angle = 0; // for camera
GLfloat angles = 0; // for the moon
CModel m;

// for image storage
Mat image[3];
GLuint tex_ids[3] = { 1, 2, 3 };
int nTex = 3;

//These draw methods are for non-imported objects
void drawMoon(){
	glPushMatrix();
	glRotatef(0 + angles, 1, 0, 0);
	glTranslatef(-250.0, 100.0, -100.0);

	glColor3f(0.980, 0.980, 0.824);
	glutSolidSphere(10, 50, 50);
	glPopMatrix();
}
void drawMagicCircle(){
	
	glPushMatrix();
	//location for magic toy ball
	glTranslatef(-170, 14, 30);
	glRotatef(40, 0, 0, 1);
	glTranslatef(0, 0, -25);
	glRotatef(90, 1.0, 0.0, 0.0);
	glRotatef(1 + ballAngle, 0.0, 0.0, 1.0);
	glTranslatef(0, -1, 0.0);
	//draw and scale
	glScalef(0.5, 0.5, 0.5);
	glutSolidSphere(1, 50, 50);
	glPopMatrix();
}

void drawSnowMan() {
	glPushMatrix();
	//snowman location
	glTranslatef(-70.0, 0.0, -11.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glColor3f(1.0f, 1.0f, 1.0f);
	glScalef(2.0, 2.0, 2.0);
	// Draw snowman body
	glTranslatef(0.0f, 0.75f, 0.0f);
	glutSolidSphere(0.75f, 20, 20);
	// Draw head
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.25f, 20, 20);
	// Draw eyes
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	glTranslatef(0.05f, 0.10f, 0.18f);
	glutSolidSphere(0.05f, 10, 10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f, 10, 10);
	glPopMatrix();
	// nose
	glColor3f(1.0f, 0.5f, 0.5f);
	glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
	glutSolidCone(0.09f, 0.5f, 10, 2);
	glPopMatrix();

}


void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camx, camy, camz,
		camx+cam2x, camy+cam2y, camz+cam2z, 
		cam_upx, cam_upy, cam_upz);
	
	
	GLfloat x, y, z, nx, ny, nz, tx, ty;
	int v_id, vt_id, vn_id;

	glBindTexture(GL_TEXTURE_2D, tex_ids[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[2].cols, image[2].rows, 0, GL_RGB, GL_UNSIGNED_BYTE, image[2].data);

	drawMagicCircle();
	drawSnowMan();
	drawMoon();
	



	for (int o = 0; o < m.objs.size(); o++){
		//coloring and texturing on each different object
		if (o == 0){ // telephone side cover
			glBindTexture(GL_TEXTURE_2D, tex_ids[1]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[1].cols, image[1].rows, 0, GL_RGB, GL_UNSIGNED_BYTE, image[1].data);
			glColor3f(0.502, 0.000, 0.000);
			
		}
		else if (o == 1){ // telephone top
			glBindTexture(GL_TEXTURE_2D, tex_ids[1]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[1].cols, image[1].rows, 0, GL_RGB, GL_UNSIGNED_BYTE, image[1].data);
			glNormal3f(0, 1, 0);
			glColor3f(0.545, 0.000, 0.000);
		}
		else if (o == 2){ // telephone middle
			glBindTexture(GL_TEXTURE_2D, tex_ids[1]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[1].cols, image[1].rows, 0, GL_RGB, GL_UNSIGNED_BYTE, image[1].data);
			glNormal3f(0, 1, 0);
			glColor3f(0.698, 0.133, 0.133);
		}
		else if (o == 3){ // telephone door
			glBindTexture(GL_TEXTURE_2D, tex_ids[1]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[1].cols, image[1].rows, 0, GL_RGB, GL_UNSIGNED_BYTE, image[1].data);
			glNormal3f(0, 1, 0);
			glColor3f(1.0, 0, 0);
		}
		else if (o == 4){ // telephone light
			glBindTexture(GL_TEXTURE_2D, tex_ids[2]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[2].cols, image[2].rows, 0, GL_RGB, GL_UNSIGNED_BYTE, image[2].data);
			glNormal3f(0, 1, 0);
			glColor3f(1.000, 0.980, 0.804
				);
		}
		else if (o == 5){ // ground
			glBindTexture(GL_TEXTURE_2D, tex_ids[1]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[1].cols, image[1].rows, 0, GL_RGB, GL_UNSIGNED_BYTE, image[1].data);
			glNormal3f(0, 1, 0);
			glColor3f(0.439, 0.502, 0.565);
		}
		else if (o == 6){ //sidewall in restuarant
			glBindTexture(GL_TEXTURE_2D, tex_ids[2]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[2].cols, image[2].rows, 0, GL_RGB, GL_UNSIGNED_BYTE, image[2].data);
			glNormal3f(0, 1, 0);
			glColor3f(0.412, 0.412, 0.412);
		}
		else if (o == 7){ // side cube
			glNormal3f(0, 1, 0);
			glColor3f(0.980,0.941, 0.902);
		}
		else if (o == 8){ // entrance cube
			glBindTexture(GL_TEXTURE_2D, tex_ids[2]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[2].cols, image[2].rows, 0, GL_RGB, GL_UNSIGNED_BYTE, image[2].data);
			glNormal3f(0, 1, 0);
			glColor3f(0.957, 0.643, 0.376
				);
		}
		else if (o == 9){ // restaraunt body
			glBindTexture(GL_TEXTURE_2D, tex_ids[0]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0].cols, image[0].rows, 0, GL_RGB, GL_UNSIGNED_BYTE, image[0].data);
			glNormal3f(0, 1, 0);
			glColor3f(0.980, 0.980, 0.824);

				
		}
		else if (o == 10){ // restaurnt entrance top cube
			glBindTexture(GL_TEXTURE_2D, tex_ids[2]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[2].cols, image[2].rows, 0, GL_RGB, GL_UNSIGNED_BYTE, image[2].data);
			glNormal3f(0, 1, 0);
			glColor3f(1.000, 0.498, 0.314
				);
		}
		else if (o == 11){ // res.floor
		
			glNormal3f(0, 1, 0);
			glColor3f(0.871, 0.722, 0.529);
		}
		else if (o == 12){ // back _desk
			glBindTexture(GL_TEXTURE_2D, tex_ids[2]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[2].cols, image[2].rows, 0, GL_RGB, GL_UNSIGNED_BYTE, image[2].data);
			glNormal3f(0, 1, 0);
			glColor3f(0.545, 0.271, 0.075);
		}
		else if (o == 13){ // book
			glNormal3f(0, 1, 0);
			glColor3f(1.000, 1.000, 0.878);
		}
		else if (o == 14){ // book
			glNormal3f(0, 1, 0);
			glColor3f(0.878, 1.000, 1.000);
		}
		else if (o == 15){ // book
			glNormal3f(0, 1, 0);
			glColor3f(0.957, 0.643, 0.376);
		}
		else if (o == 16){ // book
			glNormal3f(0, 1, 0);
			glColor3f(0.871, 0.722, 0.529);
		}
		else if (o == 17){ // book
			glNormal3f(0, 1, 0);
			glColor3f(1.000, 0.753, 0.796);
		}
		else if (o == 18){ // book
			glNormal3f(0, 1, 0);
			glColor3f(1.000, 1.000, 0.000
				);
		}
		else if (o == 19){ // book
			glNormal3f(0, 1, 0);
			glColor3f(1.000, 0.647, 0.000
				);
		}
		else if (o == 20){ // book
			glNormal3f(0, 1, 0);
			glColor3f(1.000, 0.627, 0.478
				);
		}
		else if (o == 21){ // book
			glNormal3f(0, 1, 0);
			glColor3f(0.878, 1.000, 1.000
				);
		}
		else if (o == 22){ // book
			glNormal3f(0, 1, 0);
			glColor3f(0.482, 0.408, 0.933
				);
		}
		else if (o == 23){ // book
			glNormal3f(0, 1, 0);
			glColor3f(0.737, 0.561, 0.561
				);
		}
		else if (o == 24){ // book black
			glBindTexture(GL_TEXTURE_2D, tex_ids[2]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[2].cols, image[2].rows, 0, GL_RGB, GL_UNSIGNED_BYTE, image[2].data);
			glNormal3f(0, 1, 0);
			glColor3f(0.184, 0.310, 0.310
				);
		}
		else if (o == 25){ // book
			glNormal3f(0, 1, 0);
			glColor3f(1.000, 0.894, 0.882
				);
		}
		else if (o == 26){ // book
			glNormal3f(0, 1, 0);
			glColor3f(1.000, 0.627, 0.478
				);
		}
		else if (o == 27){ // book
			glNormal3f(0, 1, 0);
			glColor3f(0.678, 1.000, 0.184

				);
		}
		else if (o == 28){ // toy
			glNormal3f(0, 1, 0);
			glColor3f(0.545, 0.271, 0.075);
		}
		else if (o == 29){ // desks
			glBindTexture(GL_TEXTURE_2D, tex_ids[0]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0].cols, image[0].rows, 0, GL_RGB, GL_UNSIGNED_BYTE, image[0].data);

			glNormal3f(0, 1, 0);
			glColor3f(0.992, 0.961, 0.902);
		}
		else if (o == 30){ // chair
			glNormal3f(0, 1, 0);
			glColor3f(0.804, 0.522, 0.247	);
		}
		else if (o == 31){ // light sphere
			glNormal3f(0, 1, 0);
			glColor3f(0.412, 0.412, 0.412
				);
		}
		else if (o == 32){ // light restaurant
			glBindTexture(GL_TEXTURE_2D, tex_ids[2]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[2].cols, image[2].rows, 0, GL_RGB, GL_UNSIGNED_BYTE, image[2].data);
			glNormal3f(0, 1, 0);
			glColor3f(1.000, 0.894, 0.710
				);
		}
		
		else{
			
			glColor3f(1, 1, 1);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//drawing by reading vectors
		int nFaces = m.objs[o].f.size();
		for (int k = 0; k < nFaces; k++) {
			int nPoints = m.objs[o].f[k].v_pairs.size();
			glBegin(GL_POLYGON);
			for (int i = 0; i < nPoints; i++) {
				v_id = m.objs[o].f[k].v_pairs[i].d[0];
				vt_id = m.objs[o].f[k].v_pairs[i].d[1];
				vn_id = m.objs[o].f[k].v_pairs[i].d[2];
				x = m.objs[o].v[v_id - 1].d[0];
				y = m.objs[o].v[v_id - 1].d[1];
				z = m.objs[o].v[v_id - 1].d[2];

				nx = m.objs[o].vn[vn_id - 1].d[0];
				ny = m.objs[o].vn[vn_id - 1].d[1];
				nz = m.objs[o].vn[vn_id - 1].d[2];
				
				tx = m.objs[o].vt[vt_id - 1].d[0];
				ty = m.objs[o].vt[vt_id - 1].d[1];
			//draw
				glNormal3f(nx, ny, nz);
				glTexCoord2f(tx,ty);
				glVertex3f(x, y, z);
				
			}
			glEnd();
			
		}
	}
	glutSwapBuffers();	
}
//timer for magic ball and moon
void timer(int){
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer, 0);
	ballAngle += 5.0f;
	angles += 0.05f;
	
}
//reshape
void reshape(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat ratio = (float)width / height;
	gluPerspective(40, ratio, 0.1, 1000);

}
//for camera esc button
void processNormalKeys(unsigned char key, int x, int y) {

	if (key == 27)
		exit(0);
}
//for camera
void processSpecialKeys(int key, int xx, int yy) {

	float fraction = 1.1f;
	switch (key) {
	case GLUT_KEY_LEFT:
		angle -= 0.1f;
		cam2x = sin(angle);
		cam2z = -cos(angle);
		break;
	case GLUT_KEY_RIGHT:
		angle += 0.1f;
		cam2x = sin(angle);
		cam2z = -cos(angle);
		break;
	case GLUT_KEY_UP:
		camx += cam2x * fraction;
		camz += cam2z * fraction;
		break;
	case GLUT_KEY_DOWN:
		camx -= cam2x * fraction;
		camz -= cam2z * fraction;
		break;
	case GLUT_KEY_PAGE_UP:
		
		camy += cam2y + fraction;
		break;
	case GLUT_KEY_PAGE_DOWN:
		camy -= cam2y + fraction;
		break;
	}
}

void init_texture(){
	glGenTextures(nTex, tex_ids);
	glEnable(GL_TEXTURE_2D);
	
}


void main(int argc, char **argv) {


	/// Following four paths should be changed as stated in the report
	//obj
	string filepath = "C:/Users/Jaesung Park/Desktop/pjs/Assignment1_JaesungPark/obj_files/final4.obj";
	ifstream fin(filepath);
	
	//texture
	image[0] = imread("C:/Users/Jaesung Park/Desktop/pjs/Assignment1_JaesungPark/texture/wood.jpg",IMREAD_COLOR); //wood texture
	cvtColor(image[0], image[0], COLOR_BGR2RGB); // to change from bgr to rgb

	image[1] = imread("C:/Users/Jaesung Park/Desktop/pjs/Assignment1_JaesungPark/texture/texture.jpg", IMREAD_COLOR); // rough texture
	cvtColor(image[1], image[1], COLOR_BGR2RGB);

	image[2] = imread("C:/Users/Jaesung Park/Desktop/pjs/Assignment1_JaesungPark/texture/plain.jpg", IMREAD_COLOR); //plain texture
	cvtColor(image[2], image[2], COLOR_BGR2RGB);

	//gl
	glutInit(&argc, argv);
	//glut double for animation
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(_WINDOW_WIDTH, _WINDOW_HEIGHT);
	glutInitWindowPosition(300, 200);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glutCreateWindow("TelephoneBox");
	m.loadObj(fin);
	fin.close();
	
	InitLight();
	init_texture();
	glutTimerFunc(0, timer, 0);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutIdleFunc(display);
	glEnable(GL_DEPTH_TEST);
	//loop
	glutMainLoop();
}