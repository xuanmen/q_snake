/*
*Author :Tharindra Galahena
*Project:snake game playing AI (neural netwark + Q learning)
*Date   :02/09/2012
*License:
* 
*     Copyright 2012 Tharindra Galahena
*
* This program is free software: you can redistribute it and/or modifood_y it under the terms of 
* the GNU General Public License as published by the Free Software Foundation, either 
* version 3 of the License, or (at your option) any later version. This program is distributed
* in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General 
* Public License for more details.
*
* You should have received a copy of the GNU General Public License along with This program. 
* If not, see http://www.gnu.org/licenses/.
*
*/

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdio.h>


#include "neural_net.cpp"

using namespace std;

typedef struct sq{
	int x;
	int y;
	int mx;
	int my;
	struct sq *next;
} sq;

sq *snake = NULL;


int      mx;
int      my;
neural *net;

int food_x      =     -6;
int food_y      =     -6;
int sc          =      0;
int num_layers  =      2;
int num_inputs  =      6;
int num_outputs =      1;
int iter        =      0;
int tmp         =     50;
int th          =      0;

bool p          =  false;
bool reseting   =   true;
bool leaning    =   true;
int	 ex  		= 	  10;

void add(int x, int y){
	sq *tmp = (sq *)malloc(sizeof(sq));
	tmp -> x = x;
	tmp -> y = y;
	tmp -> mx = 1;
	tmp -> my = 0;
	tmp -> next = snake;
	snake = tmp;
}
bool check_body(int x, int y){
	if(x == snake -> next -> x && y == snake -> next -> y) return true;
	return false;
}
sq *get_last(){
	sq *p = snake;
	while(p -> next != NULL) p = p -> next;
	return p;
}
void start(){
	snake = NULL;
	add(0, 0);
	add(1, 0);
	add(2, 0);
	add(3, 0);
	add(4, 0);
	mx = 1;
	my = 0;
}
void set_f(){
	bool f = true;
	while(f){
		srand(time(NULL));
		food_x = (rand() % 34) - 17;	 
		srand(time(NULL));
		food_y = (rand() % 34) - 17;
		sq *p = snake;
		while(p != NULL){
			if(p -> x == food_x && p -> y == food_y){
				f = true;
				break;		
			}	
			f = false;
			p = p -> next;
		}	
	}
}
bool tail(){
	sq *p = snake;
	while(p -> next != NULL){
		if(p -> next -> x == snake -> x && p -> next -> y == snake -> y)
			return true;	
		p = p -> next;
	}
	return false;
}
bool tail2(int sx, int sy){
	sq *p = snake;
	while(p -> next != NULL){
		if(p -> next -> x == sx && p -> next -> y == sy)
			return true;	
		p = p -> next;
	}
	return false;
}
float check2(int x, int y){
	sq *p = snake;
	while(p -> next != NULL){
		if(p -> next -> x == x && p -> next -> y == y)
			return -1.0;	
		p = p -> next;
	}
	if(x > 18 || x < -18 || y > 18 || y < -18) return -1.0;
	return 1.0;
}
float check(int x, int y){
	sq *p = snake;
	while(p != NULL){
		if(p -> x == x && p -> y == y)
			return -1.0;	
		p = p -> next;
	}
	if(x > 18 || x < -18 || y > 18 || y < -18) return -1.0;
	return 1.0;
}
void rev(){
	sq *snake2 = NULL;
	sq *p = snake;
	while(p != NULL){
		sq *tmp = (sq *)malloc(sizeof(sq));
		tmp -> x = p -> x;
		tmp -> y = p -> y;
		tmp -> mx = -1 * p -> mx;
		tmp -> my = -1 * p -> my;
		tmp -> next = snake2;
		snake2 = tmp;
		sq *x = p -> next;
		free(p);
		p = x;	
	}
	snake = snake2;
	mx = snake -> mx;
	my = snake -> my;
}

void move(){
	sq *p = snake;
	int x = p -> x;
	int y = p -> y;
	int tmx = p -> mx;
	int tmy = p -> my;
	while(p -> next != NULL){
		sq *q = p -> next;
		int tmp = q -> x;
		q -> x = x;
		x = tmp;

		tmp = q -> y;
		q -> y = y;
		y = tmp;

		tmp = q -> mx;
		q -> mx = tmx;
		tmx = tmp;

		tmp = q -> my;
		q -> my = tmy;
		tmy = tmp;

		p = p -> next;
	}
	snake -> mx = mx;
	snake -> my = my;
	snake -> x += mx;
	snake -> y += my;
}
void par(float x1, float x2, float y1, float y2, float z1, float z2){
	if(leaning) glColor3f(1.0, 0.0, 1.0);
	else  glColor3f(1.0, 1.0, 0.0);
	
	glBegin(GL_QUADS);
	
	glVertex3f(x1, y1, z1);
	glVertex3f(x2, y1, z1);
	glVertex3f(x2, y2, z1);
	glVertex3f(x1, y2, z1);

	glEnd();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity ();

	glTranslatef(0.0, 0.0, -22.0);
	int i;
	sq *p = snake;
	par(-8.7,  9.2,  9.0,  9.2, 0.0, 0.0);
	par(-8.7,  9.2, -8.5, -8.7, 0.0, 0.0);
	par(-8.5, -8.7, -8.7,  9.2, 0.0, 0.0);
	par( 9.2,  9.0, -8.7,  9.2, 0.0, 0.0);
	while(p != NULL){
		par((p -> x)/2.0,(p -> x)/2.0 + 0.4,(p -> y)/2.0,(p -> y)/2.0 + 0.4, 0.0, 0.0);
		p = p -> next;	
	}
	par(food_x/2.0, food_x/2.0 + 0.4 , food_y/2.0 , food_y/2.0 + 0.4, 0.0 , 0.0);
	glutSwapBuffers();
}
float reward(int sx, int sy, int sx1, int sy1){
	if(snake -> x == food_x && snake -> y == food_y){
		add(food_x, food_y);	
		sc++;
		cout << "================= > score = " << sc << endl;
		set_f();
		return 1000.0;
	}else if(tail()){
		sc = 0;
		start();
		cout << "----END---- t" << endl;
		//getchar();
		return -100000.0;
	}else if(snake -> x > 18 || snake -> x < -18 || snake -> y > 18 || snake -> y < -18){
		//cout << "----END----" << endl;
		start();
		return -1000.0;
	}
	float re2 = sqrt((sx1 - food_x) * (sx1 - food_x) + (sy1 - food_y) * (sy1 - food_y)); 
	return -re2;
}
float reward2(int sx, int sy){
	if(sx == food_x && sy == food_y){
		return 1000.0;
	}else if(tail2(sx, sy)){
		return -10000.0;
	}else if(sx > 18 || sx < -18 || sy > 18 || sy < -18){
		return -1000.0;
	}
	float re = sqrt((sx - food_x) * (sx - food_x) + (sy - food_y) * (sy - food_y)); 
	return -1.0 * re;	
}
float *get_q(int sx, int sy){
	float inputs[6];
	inputs[0] = sqrt((sx - food_x) * (sx - food_x) + (sy - food_y) * (sy - food_y)); 
	inputs[1] = check(sx	, sy	);
	inputs[2] = check(sx + 1, sy	);
	inputs[3] = check(sx    , sy + 1);
	inputs[4] = check(sx - 1, sy);
	inputs[5] = check(sx    , sy - 1);
	return net -> feed(inputs);
}
float max_q(int sx, int sy, int food_x, int food_y){
	
	float new_q = 0.0;
	int sx1 = sx + 1;
	int sy1 = sy;
	if(check_body(sx1, sy1)){
		sq *last = get_last();
		sx1 = last -> x - last -> mx;
		sy1 = last -> y - last -> my;
	}
		
	float *out1 = get_q(sx1, sy1);
			
	sx1 = sx - 1;
	sy1 = sy;
	if(check_body(sx1, sy1)){
		sq *last = get_last();
		sx1 = last -> x - last -> mx;
		sy1 = last -> y - last -> my;
	}		
			
	float *out2 = get_q(sx1, sy1);
			
	sx1 = sx;
	sy1 = sy + 1;
	if(check_body(sx1, sy1)){
		sq *last = get_last();
		sx1 = last -> x - last -> mx;
		sy1 = last -> y - last -> my;
	}		
	
	float *out3 = get_q(sx1, sy1);
			
	sx1 = sx;
	sy1 = sy - 1;
	if(check_body(sx1, sy1)){
		sq *last = get_last();
		sx1 = last -> x - last -> mx;
		sy1 = last -> y - last -> my;
	}		
			
	float *out4 = get_q(sx1, sy1);
	
	if(out1[0] > out2[0]){
		if(out1[0] > out3[0]){
			if(out1[0] > out4[0]){
				new_q = out1[0];
				if(mx == -1) rev();
				else{
					mx =  1;
					my =  0;
				}
			}else{
				new_q = out4[0];
				if(my == 1) rev();
				else{
					mx =  0;
					my = -1;
				}
			}	
		}else{
			if(out3[0] > out4[0]){
				new_q = out3[0];
				if(my == -1) rev();
				else{
					mx =  0;
					my =  1;
				}
			}else{
				new_q = out4[0];
				if(my == 1) rev();
				else{
					mx =  0;
					my = -1;
				}
			}
		}
	}else{
		if(out2[0] > out3[0]){
			if(out2[0] > out4[0]){
				new_q = out2[0];
				if(mx == 1) rev();
				else{
					mx = -1;
					my =  0;
				}
			}else{
				new_q = out4[0];
				if(my == 1) rev();
				else{
					mx =  0;
					my = -1;
				}
			}	
		}else{
			if(out3[0] > out4[0]){
				new_q = out3[0];
				if(my == -1) rev();
				else{
					mx =  0;
					my =  1;
				}
			}else{
				new_q = out4[0];
				if(my == 1) rev();
				else{
					mx =  0;
					my = -1;
				}
			}
		}
	}
	return new_q;
}
/*
float max_q2(int sx, int sy, int food_x, int food_y){
	float new_q = 0.0;
	int sx1 = sx + 1;
	int sy1 = sy;
	if(check_body(sx1, sy1)){
		sq *last = get_last();
		sx1 = last -> x - last -> mx;
		sy1 = last -> y - last -> my;
	}
		
	float *out1 = get_q(sx1, sy1);
			
	sx1 = sx - 1;
	sy1 = sy;
	if(check_body(sx1, sy1)){
		sq *last = get_last();
		sx1 = last -> x - last -> mx;
		sy1 = last -> y - last -> my;
	}		
			
	float *out2 = get_q(sx1, sy1);
			
	sx1 = sx;
	sy1 = sy + 1;
	if(check_body(sx1, sy1)){
		sq *last = get_last();
		sx1 = last -> x - last -> mx;
		sy1 = last -> y - last -> my;
	}		
		
	float *out3 = get_q(sx1, sy1);
			
	sx1 = sx;
	sy1 = sy - 1;
	if(check_body(sx1, sy1)){
		sq *last = get_last();
		sx1 = last -> x - last -> mx;
		sy1 = last -> y - last -> my;
	}		
			
	float *out4 = get_q(sx1, sy1);	
	
	if(out1[0] > out2[0]){
		if(out1[0] > out3[0]){
			if(out1[0] > out4[0]){
				new_q = out1[0];
				
			}else{
				new_q = out4[0];
				
			}	
		}else{
			if(out3[0] > out4[0]){
				new_q = out3[0];
				
			}else{
				new_q = out4[0];
				
			}
		}
	}else{
		if(out2[0] > out3[0]){
			if(out2[0] > out4[0]){
				new_q = out2[0];
				
			}else{
				new_q = out4[0];
				
			}	
		}else{
			if(out3[0] > out4[0]){
				new_q = out3[0];
				
			}else{
				new_q = out4[0];
				
			}
		}
	}
	return new_q;
}
*/
float old_q = 0.0;
void itera(){
	th++;
	int sx = snake -> x;
	int sy = snake -> y;
		
	float inputs[6];
	int sx1 = sx;
	int sy1 = sy;
	
	//float *out = get_q(sx1, sy1);
		
	float new_q;
	if(rand() % 100 > ex){
		new_q = max_q(sx, sy, food_x, food_y);
	}else{
		int a = rand() % 4;
		if(a == 0){
			sx1 = sx + 1;
			sy1 = sy;
			if(check_body(sx1, sy1)){
				sq *last = get_last();
				sx1 = last -> x - last -> mx;
				sy1 = last -> y - last -> my;
			}	
			if(mx == -1) rev();
			else{
				mx =  1;
				my =  0;
			}
		}else if(a == 1){
			sx1 = sx - 1;
			sy1 = sy;
			if(check_body(sx1, sy1)){
				sq *last = get_last();
				sx1 = last -> x - last -> mx;
				sy1 = last -> y - last -> my;
			}
			if(mx == 1) rev();
			else{
				mx = -1;
				my =  0;
			}
		}else if(a == 2){
			sx1 = sx;
			sy1 = sy + 1;
			if(check_body(sx1, sy1)){
				sq *last = get_last();
				sx1 = last -> x - last -> mx;
				sy1 = last -> y - last -> my;
			}
			if(my == -1) rev();
			else{
				mx =  0;
				my =  1;
			}
		}else{
			sx1 = sx;
			sy1 = sy - 1;
			if(check_body(sx1, sy1)){
				sq *last = get_last();
				sx1 = last -> x - last -> mx;
				sy1 = last -> y - last -> my;
			}
			if(my == 1) rev();
			else{
				mx =  0;
				my = -1;
			}
		}
		
		float *out1 = get_q(sx1, sy1);
		new_q = out1[0];
	}
	get_q(sx1, sy1);
	move();	
	sx1 = snake -> x;
	sy1 = snake -> y;
	
	float dout[1];
	//new_q = max_q2(sx1, sy1, food_x, food_y);
	float re = reward(sx, sy, sx1, sy1);
	dout[0] =  re + 0.9 * new_q - old_q;
	net -> learn(dout);
	old_q = new_q;
}
void myIdleFunc(int a) {
	if(!p){
		itera();
	}
	cout << "------------------------------------------------------" << th << " " << ex << endl;
	glutPostRedisplay();
	glutTimerFunc(tmp, myIdleFunc, 0);
}
void keyboard(unsigned char key, int x, int y)
{
	if((char)key == 'p'){
		if(p) p = false;
		else p = true;	
	}else if((char)key == 't'){
		tmp--;
	}else if((char)key == 'y'){
		if(tmp < 0) tmp = 0;
		tmp++;
	}else if((char)key == 's'){
		set_f();
	}else if((char)key == 'f'){
		reseting = false;
	}else if((char)key == 'r'){
		reseting = true;
	}else if((char)key == 'l'){
		leaning = true;
	}else if((char)key == 'n'){
		leaning = false;
	}else if((char)key == 'e'){
		ex++;
	}else if((char)key == 'h'){
		ex--;
	}else if((char)key == 'q'){
		p = true;
		for(int i = 0; i < 500; i++) itera();
		p = false;
	}
}

void init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);	
	glLoadIdentity ();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	GLfloat acolor[] = {1.4, 1.4, 1.4, 1.0};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, acolor);

}
void Reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	gluPerspective(45.0, (float)w/(float)h, 0.1, 200.0);
	
}
int main(int argc, char** argv)
{
	srand(time(NULL));
	net = new neural(num_inputs, num_outputs, num_layers, 10, 0.0000001, 0.8);
	net -> init();
	start();
	set_f();
	glutInit(&argc,argv);
	glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(450,450);
	glutInitWindowPosition(500,0);
	glutCreateWindow("snake game automation");
	init();
	glutTimerFunc(400, myIdleFunc, 0);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc( keyboard );
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}

