#include<stdio.h>
#include<gl/glut.h>
#include<gl/GLU.h>
#include<gl/GL.h>

static int SpinAngle = 0;
int width, height;
typedef struct i {
	unsigned char *data;
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int w, h;
	unsigned int imageSize;
}IData;
IData bmap;
GLuint imgTexture;

GLuint LoadTexture(const char * filename) {//파일에서 텍스처 불러오기
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int w, h;
	unsigned int imageSize;
	unsigned char *data;

	FILE *file = fopen(filename, "rb");
	if (!file) {
		printf("Image could not be opened\n");
		return 0;
	}
	if (fread(header, 1, 54, file) != 54) {
		printf("Not a correct BMP file\n");
		return false;
	}
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		return 0;
	}
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	w = *(int*)&(header[0x12]);
	h = *(int*)&(header[0x16]);

	if (imageSize == 0) imageSize = w*h * 3;
	if (dataPos == 0) dataPos = 54;
	data = new unsigned char[imageSize];
	fread(data, 1, imageSize, file);
	bmap.data = data;
	bmap.dataPos = dataPos;
	bmap.h = h;
	bmap.w = w;
	bmap.imageSize = imageSize;

	fclose(file);

	return 0;
}
void MakeTexture() {		//텍스쳐 만들기
	glBindTexture(GL_TEXTURE_2D, imgTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmap.w, bmap.h, 0, GL_RGB, GL_UNSIGNED_BYTE, bmap.data);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}
void Init() {//초기화함수
	GLfloat light0_ambient[] = { 0.8,0.4,0.09, 1.0 };     //조명 특성
	GLfloat light0_diffuse[] = { 1.0,0.8,0.09, 1.0 };
	GLfloat light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	GLfloat material_ambient[] = { 0.4,0.4,0.4, 1.0 };  //물체 특성
	GLfloat material_diffuse[] = { 0.8,0.8,0.8, 1.0 };
	GLfloat material_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat material_shininess[] = { 25.0 };

	glShadeModel(GL_SMOOTH);    //구로 셰이딩
	glEnable(GL_DEPTH_TEST);    //깊이 버퍼 활성화
	glEnable(GL_LIGHTING);      //조명 활성화
	glEnable(GL_LIGHT0);
	glEnable(GL_TEXTURE_2D);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient); //light0에 특성 할당
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-5.0, 5.0, -5.0, 5.0, -5.0, 5.0);//parallel projection을 시키는 것
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void drawCar() {
	glPushMatrix();
	glPushMatrix();                   // body
	glScalef(5.5, 1.5, 3);
	glutSolidCube(.5);

	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, .375, 0);
	glutSolidSphere(.5, 20, 20);
	glutWireSphere(.7,10,10);
	glPopMatrix();
	glTranslatef(0, 0, .75);
	glPushMatrix();
	glTranslatef(-1.0, -0.6, 0);
	glutSolidTorus(.15, .3, 24,24);       // wheel
	glTranslatef(2.0, 0, 0);
	glutSolidTorus(.15, .3, 24, 24);       // wheel
	glPopMatrix();
	glTranslatef(0, 0, -1.5);
	glPushMatrix();
	glTranslatef(-1.0, -.6, 0);
	glutSolidTorus(.15, .3, 24, 24);       // wheel
	glTranslatef(2.0, 0, 0);
	glutSolidTorus(.15, .3, 24, 24);       // wheel
	glPopMatrix();
	glPopMatrix();
}
void DrawScene() {//teapot과 light material 구현
	GLfloat LightPosition[] = { 0.0, 0.0, 1.5, 1.0 };
	glPushMatrix();
	glTranslatef(0.0, 0.0, -5.0);
	glPushMatrix();
	glRotatef(SpinAngle, 1.0, 0.0, 0.0);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	glTranslatef(0.0, 0.0, 1.5);
	glDisable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 1.0);
	glutWireCone(0.1,0.1, 10, 10);//wirecone라는 모델로 조명을 구현
	glEnable(GL_LIGHTING);
	glPopMatrix();
	drawCar();
	glPopMatrix();
	glFlush();
}
void MyDisplay() {//display callback 4개로 구역 나눔
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//frame buffer
	glColor3f(1.0, 1.0, 1.0);
	//projection 거리 5
	glViewport(0, 0, width / 2, height / 2);	//좌하단-front
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -5.0, 0.0, 1.0, 0.0);//카메라의 보는 위치
	DrawScene();
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glViewport(width / 2, 0, width / 2, height / 2);	//우하단-side
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	gluLookAt(5.0, 0.0, -5.0, 0.0, 0.0, -5.0, 0.0, 1.0, 0.0);
	DrawScene();
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glViewport(0, height / 2, width / 2, height / 2);//좌상단-top
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	gluLookAt(0.0, 5.0, -5.0, 0.0, 0.0, -5.0, 0.0, 0.0, -7.0);//카메라의 보는 위치
	DrawScene();
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glViewport(width / 2, height / 2, width / 2, height / 2);//우상단
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPerspective(30, 1.0, 1.0, 50.0);//perspective projection사용, 다른것들은 기본으로 glOrtho로 인해서 설정된 parallel projection사용
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	gluLookAt(5.0, 5.0, 0.0, 0.0, 0.0, -5.0, 0.0, 1.0, 0.0);
	DrawScene();
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glFlush();
}
void MyReshape(int w, int h) {//윈도우사이즈 조정했을때
	width = w;
	height = h;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void MyMouse(int button, int state, int x, int y) {//마우스 눌렀을때
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			SpinAngle = (SpinAngle + 15) % 360;
			glutPostRedisplay();
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			SpinAngle = (SpinAngle - 15);
			if (SpinAngle <= 0) {
				SpinAngle += 360;
			}
			glutPostRedisplay();
		}
		break;
	default:
		break;
	}
}
int main(int argc, char **argv) {
	width = 500;
	height = 500;;
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("16011087 LeeSoobin CG homework");
	Init();
	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(MyReshape);
	glutMouseFunc(MyMouse);

	glGenTextures(1, &imgTexture);
	LoadTexture("bricks.bmp");
	MakeTexture();

	glutMainLoop();
	return 0;
}