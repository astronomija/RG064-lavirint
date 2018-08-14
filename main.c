#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>

static int window_width,window_height;
int sirina,duzina;
int res = 0;
char *lav;

static void on_keyboard(unsigned char key,int x ,int y);
static void on_reshape(int width,int height);
static void on_display(void);


static void on_keyboard(unsigned char key,int x,int y)
{
	switch(key){
		case 27:
			exit(0);
			break;
	}
}	

static void on_reshape(int width,int height)
{
	window_width = width;
	window_height = height;
	glViewport(0,0,window_width,window_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1,1,-1,1,1,10);
	
}

void prikaz(const char *lav,int sirina,int duzina)
{
	int x,y;
	for(y=0; y<duzina; y++)
	{
		for(x = 0; x<sirina; x++)
		{
			switch(lav[y*sirina +x]){
				case 1: {
				glColor4f(0,0,0, 0);
                glPushMatrix();
                glTranslatef(x * 2 - 2, 0, y * 2);
                glutSolidCube(1);
                glPopMatrix();
            }; break;
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

	/*secenje/oblikovanje*/

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

   /* uklanjanje pocetka i kraja */
   lav[0 * sirina + 1] = 1;
   lav[(duzina - 1) * sirina + (sirina - 2)] = 1;

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

   /* Replace the entry and exit. */
   lav[(duzina - 2) * sirina + (sirina - 2)] = 2;
   lav[(duzina - 1) * sirina + (sirina - 2)] = 2;

}

int main(int argc,char *argv[]){
	

	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

	glutInitWindowSize(1000,1000);
	glutInitWindowPosition(100,100);
	glutCreateWindow(argv[0]);

	glutKeyboardFunc(on_keyboard);
	glutReshapeFunc(on_reshape);
	glutDisplayFunc(on_display);


	glClearColor(1,0.9,1,1);
	glEnable(GL_DEPTH_TEST);
	glLineWidth(GL_DEPTH_TEST);
	glLineWidth(0.5);


	printf("Ucitajte dimenzije zeljenog lavirinta\n" );

	scanf("%d",&sirina);
	scanf("%d",&duzina);

	sirina = sirina *2 +3;
	duzina = duzina * 2+3;
	if(sirina < 7 || duzina < 7) {
      printf("dimenizije moraju biti vece od 7\n");
      exit(EXIT_FAILURE);
   }	

    if(argc == 2 && argv[1][0] != 's') {
      printf("error: invalid argument\n");
      exit(EXIT_FAILURE);
   }

   lav = (char*)malloc(sirina * duzina * sizeof(char));
   if(lav == NULL){
   		printf("Nema dovoljno memorije\n");
   		exit(EXIT_FAILURE);
   }

   printf("\n");
   if(argc == 2){
   		res = 1;

   }
     glutMainLoop();
  
   free(lav);
 
   exit(EXIT_SUCCESS);
}

void on_display(void){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(1,4,20,0,0,0,0,1,0);

   generisanje(lav,sirina,duzina);
   prikaz(lav,sirina,duzina);


   glutSwapBuffers();

}