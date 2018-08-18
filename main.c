#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>

static int window_width, window_height;
static float move_up,move_side;
static int timer_active;


static void initialize(void);

static void on_keyboard(unsigned char key,int x,int y);
static void on_display(void);
static void on_timer(int value);
static void on_reshape(int width,int window_height);
int sirina,duzina;
char *lav;

void prikaz(const char *lav,int sirina,int duzina);
void oblikuj(char *lav,int sirina,int duzina,int x, int y);
void generisanje(char *lav , int sirina,int duzina);
void resenje(char *lav, int sirina, int duzina);

int main(int argc,char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	glutInitWindowSize(600,600);
	glutInitWindowPosition(100,100);
	glutCreateWindow(argv[0]);

	glutKeyboardFunc(on_keyboard);
	glutReshapeFunc(on_reshape);
	glutDisplayFunc(on_display);

	duzina = 17;
	sirina=17;
	move_up=0;
	move_side=0;
	timer_active=0;
	
	initialize();
	

	
	lav = (char*)malloc(sirina*duzina *sizeof(char));
	if(lav == NULL){
		printf("nema dovoljno memorije\n");
		exit(EXIT_FAILURE);
	}
	generisanje(lav,sirina,duzina);
	prikaz(lav,sirina,duzina);
	printf("\n");
	//resenje(lav,sirina,duzina);
	glutMainLoop();
	free(lav);

	return 0;
}

static void initialize(void)
{
	glClearColor(0,0,0,1);
	glEnable(GL_DEPTH_TEST);
	
	glLineWidth(7);
	glPointSize(2);



}

static void on_keyboard(unsigned char key,int x,int y)
{
	switch(key){
		case 27:
			exit(0);
			break;
		//Prikazuje se putanja za prolaz kroz lavirint
		case 'r':
			resenje(lav,sirina,duzina);
			prikaz(lav,sirina,duzina);
			break;
		case 'g':
			if(!timer_active){
				glutTimerFunc(50,on_timer,0);
				timer_active = 1;
			}
			break;
	}
	glutPostRedisplay();
}

static void on_timer(int value)
{
	if(value != 0)
		return;

	for(int i = 0;i<sirina;i++)
		for(int j=0;j<duzina;j++)
		{	
			if(lav[i*sirina+j]==2){
				move_up=i*0.1;
				move_side=j*0.1;
				
			}
				

			
		
		}

		glutPostRedisplay();

		if(timer_active)
				glutTimerFunc(50,on_timer,0);
				
	
}

static void on_reshape(int width,int height)
{
	window_width=width;
	window_height=height;
	glViewport(0,0,window_width,window_height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40,window_width/(float)window_height,1,1500);
}

static void on_display(void)
{
	int i;

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
				1,3,2,
				0,0,0,
				0,1,0
			);
	
	

	glColor3f(0.95,0,0.96);
	glTranslatef(-0.7,-0.7,0);
	glRotatef(20,1,0,0);
	glRotatef(90,0,1,0);
	
	
	for(i = 0;i<sirina;i++)
		for(int j=0;j<duzina;j++)
		{	
			if(lav[i*sirina+j]==1){
				glPushMatrix();
				glTranslatef(i*0.1,0,j*0.1);
				glutSolidCube(0.1);
				glPopMatrix();
			}	
	}
	
	glBegin(GL_POINTS);
	for(i = 0;i<sirina;i++)
		for(int j=0;j<duzina;j++)
		{	
			glColor3f(1,1,1);
			if(lav[i*sirina+j]==2){
				glVertex3f(i*0.1,0,j*0.1);
			}	
	}
	glEnd();

	glPushMatrix();
        glTranslatef(move_up,0,move_side);
        glColor3f(0, 0, 1);
        glutWireSphere(0.1, 10, 10);
    glPopMatrix();

	



	glutSwapBuffers();
}

//prikaz lavirinta u terminalu
void prikaz(const char *lav,int sirina,int duzina)
{
	int x,y;
	for(y=0; y<duzina; y++)
	{
		for(x = 0; x<sirina; x++)
		{
			switch(lav[y*sirina +x]){
				case 1: printf("[]");
				 break;
				case 2:printf("<>"); break;
				default: printf("  "); break;
			}
		}	
		printf("\n");
	}
}

void oblikuj(char *lav,int sirina,int duzina,int x, int y)
{
	int x1,y1,x2,y2,dx,dy,pr,br;

	pr = rand() % 4;
	br = 0;

	while(br<4){
		dx = 0; dy = 0;
		switch(pr){
			case 0: dx = 1; break;
			case 1: dy = 1; break;
			case 2: dx = -1; break;
			default: dy = -1;break;
		}
		x1 = x + dx;
		y1 = y + dy;
		x2 = x1+dx;
		y2 = y1 +dy;

		if(x2>0 && x2 < sirina && y2 > 0 && y2< duzina
			&& lav[y1 * sirina +x1] == 1 && lav[y2 * sirina + x2]==1){
			lav[y1 * sirina +x1] = 0;
			lav[y2*sirina + x2] = 0;
			x = x2; y = y2;
			pr = rand() % 4;
			br=0;
		} else{
			pr = (pr +1) %4;
			br ++;
		}
		
	}
}

void generisanje(char *lav , int sirina,int duzina){
	int x,y;

	for(x = 0; x<sirina * duzina;x++)
	{
		lav[x]=1;
	}
	lav[1*sirina+1]= 0;

	srand(time(0));

	/*secenje/oblikovanje lavirinta*/

	for(y = 1; y<duzina; y+=2)
	{
		for(x = 1; x< sirina; x+=2){
			oblikuj(lav,sirina,duzina,x,y);
		}
	}
	lav[0*sirina+1]=0;
	lav[(duzina -1) * sirina + (sirina-2)] = 0;
}

void resenje(char *lav, int sirina, int duzina) {

   int pr, br;
   int x, y;
   int dx, dy;
   int forward;


   forward = 1;
   pr = 0;
   br = 0;
   x = 1;
   y = 1;
   while(x != sirina - 2 || y != duzina - 2) {
      dx = 0; dy = 0;
      switch(pr) {
      case 0:  dx = 1;  break;
      case 1:  dy = 1;  break;
      case 2:  dx = -1; break;
      default: dy = -1; break;
      }
      if(   (forward  && lav[(y + dy) * sirina + (x + dx)] == 0)
         || (!forward && lav[(y + dy) * duzina + (x + dx)] == 2)) {
         lav[y * sirina + x] = forward ? 2 : 3;
         x += dx;
         y += dy;
         forward = 1;
         br = 0;
         pr = 0;
      } else {
         pr = (pr + 1) % 4;
         br+= 1;
         if(br > 3) {
            forward = 0;
            br = 0;
         }
      }
   }

   lav[(duzina - 2) * sirina + (sirina - 2)] = 2;
   lav[(duzina - 1) * sirina + (sirina - 2)] = 2;

}
