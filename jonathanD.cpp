// Author: Jonathan Dinh
// Date written: 4/18/2016
// Last Modified: 5/02/2016
// Purpose: This file is responsible for all lilypads related functions such
// as spawning lilypads, deleting lilypads, and behaviors of lilypads
//

#include <GL/glx.h>
#include "jonathanD.h"
#include "ppm.h"
#include "game.h"
#include "kevinJ.h"
#include <string>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include "fonts.h"
int create_tcp_socket();
void usage();
char *get_ip(char *host);
char *build_get_query(char *host, char *page);

#define HOST "coding.debuntu.org"
#define PAGE "/"
#define PORT 80
#define USERAGENT "HTMLGET 1.0"

using namespace std;
struct Game game;
unsigned char *buildAlphaData2(Ppmimage *img);

float fade = 1.0;
void drawRipple(int x, int y)
{
    if (fade <= 0)
        fade = 1.0;
    float radius = 20;
    int detail = 400;
    float radian = 2.0 * 3.14;
    glEnable(GL_BLEND);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor4f (1.0,0.0,0.0,fade-=0.01 );
    glBegin ( GL_LINE_LOOP );
    for ( int i = 0; i <= detail; i++ ) {
        glVertex2f (
                x + ( radius * cos ( (float)i * radian / (float)detail ) ),
                y + ( radius * sin ( (float)i * radian / (float)detail ) )
                );
    }
    glEnd();
    glPopMatrix();
    glDisable(GL_BLEND);
}

void createLily(const int n, Game *game)
{
    int wid;
    if (game->score < 300) {
        wid = 35;
    }
    if (game->score >= 300) {
        wid = 30;
    }
    if (game->score >= 800) {
        wid = 25;
    }
    if (game->score >= 1300) {
        wid = 20;
    }
    if (game->score >= 2000) {
        wid = 15;
    }

    for (int i =0; i < n; i++) {
        Lilypad *node = new Lilypad;
        if (node == NULL) {
            //Log("error allocating node.\n");
            exit(EXIT_FAILURE);
        }
        node->size = wid;
        node->prev = NULL;
        node->next = NULL;
        int random = rand() % (game->windowWidth - 120) + 60;
        node->pos[0] = game->windowWidth - random;
        if (node->pos[0] >= game->windowWidth/2) {
            node->left = false;
        } else {
            node->left = true;
        }
        node->pos[1] = game->lilyspawnpoint;
        node->vel[1] = -2.0f;
        node->next = game->ihead;
        if (game->ihead != NULL) {
            game->ihead->prev = node;
        }
        game->ihead = node;
        game->nlily++;
    }
}

void checkLilies(Game *game)
{
    if (game->stresstest == 1) {
        game->maxtimer = 2;
    } else {
        game->maxtimer = 35;
    }
    //game timer for when to spawn new lily
    game->timer++;
    if (game->timer >= game->lilytimer) {
        createLily(1,game);
        game->nlily++;
        game->timer = 0;
    }
    //lilies falling down
    Lilypad *node = game->ihead;
    while (node) {
        node->pos[1] += node->vel[1];
        //delete lily if it falls below certain height
        if (node->pos[1] <= 0.0) {
            deleteLily(node, game);
        }
        node = node->next;
        //check y pos to set score to 0
        if (game->c.center[1] == 15.0) {

        }
    }
}

void deleteLily(Lilypad *node, Game *game)
{
    if (node) {
        //cout << node->pos[0] << " " << node->pos[1] << endl;
        drawRipple((int)node->pos[0],(int)node->pos[1]);
        if (node->next == NULL && node->prev == NULL) {
            game->ihead = NULL;
            //delete(node);
            //node = NULL;
        } else if (node->next && node->prev) {
            node->prev->next = node->next;
            node->next->prev = node->prev;
            delete(node);
            node = NULL;
        } else if (node->prev == NULL && node->next) {
            game->ihead = node->next;
            node->next->prev = NULL;
            delete(node);
            node = NULL;
        } else if (node->next == NULL && node->prev) {
            node->prev->next = NULL;
            delete(node);
            node = NULL;
        }
    }
    game->nlily--;
}

void clearLilies(Game *game)
{
    Lilypad *node = game->ihead;
    while (node) {
        deleteLily(node, game);
        node = node->next;
    }
}

int check = 0;
void drawLilies(Game *game)
{
    Lilypad *node = game->ihead;
    while (node) {
        if (game->troll_lilypad == 1) {
            if (!node->left) {
                node->pos[0] += 2;
                if (node->pos[0] > game->windowWidth) {
                    node->pos[0] = game->windowWidth;
                    node->left = true;
                }

            } else {
                node->pos[0] -= 2;
                if (node->pos[0] < 0) {
                    node->pos[0] = 0;
                    node->left = false;
                }
            }
        }
        glPushMatrix();
        glTranslatef(node->pos[0], node->pos[1], 0);

        if (check>=40 && check <80) {
            //if (r>9)
            glBindTexture(GL_TEXTURE_2D, game->lily->lillyTexture[0]);
            check++;
        }
        if (check >= 80) {
            //if (r<=1)
            glBindTexture(GL_TEXTURE_2D, game->lily->lillyTexture[2]);
            check++;
        }
        if (check>100)
            check =0;

        if (check<40) {
            glBindTexture(GL_TEXTURE_2D, game->lily->lillyTexture[1]);
            check++;
        }

        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor4ub(255,255,255,255);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2i(-node->size,-node->size);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2i(-node->size, node->size);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2i( node->size, node->size);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2i( node->size,-node->size);
        glEnd();
        glPopMatrix();
        glDisable(GL_ALPHA_TEST);
        glDisable(GL_TEXTURE_2D);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_2D);
        node = node->next;
    }
}

unsigned char *buildAlphaData2(Ppmimage *img)
{
    //add 4th component to RGB stream...
    int i;
    int a,b,c;
    unsigned char *newdata, *ptr;
    unsigned char *data = (unsigned char *)img->data;
    newdata = (unsigned char *)malloc(img->width * img->height * 4);
    ptr = newdata;
    for (i=0; i<img->width * img->height * 3; i+=3) {
        a = *(data+0);
        b = *(data+1);
        c = *(data+2);
        *(ptr+0) = a;
        *(ptr+1) = b;
        *(ptr+2) = c;
        *(ptr+3) = (a|b|c);
        ptr += 4;
        data += 3;
    }
    return newdata;
}

//drawing score using sprites
void drawScore(int s, Game *game,int wid, int xpos, int ypos)
{
    if (wid==0) {
        wid = 20;
    }
    string score;
    stringstream out;
    int size;
    out << s;
    score = out.str();
    size = score.length();
    for (int i = 0; i < size; i++) {
        char cdigit = score[i];
        int idigit = cdigit - '0'; //ghetto atoi
        //draw score
        glPushMatrix();
        glTranslatef(xpos, ypos, 0);
        glBindTexture(GL_TEXTURE_2D, game->hscore->scoreTexture[idigit]);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor4ub(255,255,255,255);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2i(-wid,-wid);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2i(-wid, wid);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2i( wid, wid);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2i( wid,-wid);
        glEnd();
        glPopMatrix();
        glDisable(GL_ALPHA_TEST);
        glDisable(GL_TEXTURE_2D);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_2D);
        xpos+=30;
    }
}

int getDigits(int c)
{
    int digits = 0;
    if (c < 0)
        digits = 1;
    while (c) {
        c /= 10;
        digits++;
    }
    return digits;
}

int getMidpoint(Game *game)
{
    int wid = 20;
    int digits = getDigits(game->tempscore);
    int halfpoint = (digits*30)/2 - wid;
    return halfpoint;
}

void drawBubble(Game *game)
{
    glBindTexture(GL_TEXTURE_2D, 0);
    Rect r;
    r.bot = game->fly->getYpos()+50;
	r.left = game->fly->getXpos()+100;
	r.center = 10;
    glBindTexture(GL_TEXTURE_2D, 0);
    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, 0);
    int w = 100;
	int h = 100;
    glTranslatef(game->fly->getXpos()+w, game->fly->getYpos()+h, 0);
	glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
	ggfrog40b ( &r, 50, 0, "Ha Ha!" );


}

void flyBy(Game *game)
{
    int x=rand() %10+1;
    int y =rand() %10+1;
    x=6-x;
    y=6-y;

    game->fly->move (game->fly->getXpos(),
                    game->fly->getYpos()-10,
					game->fly->getXvel(),
					game->fly->getYvel() );

}

void initHighScore(Game *game) {
    char *host = (char*)"sleipnir.cs.csub.edu";
    char *tpage = (char*)
		"/~jhargreaves/upstream/lowScore.txt";
    //get highscore into text
    getHighScore(game, host, tpage,true,false);
}

void drawHighScoreBox(Game *game)
{
    int wid = 190.0;
    int height = 110.0;
    glPushMatrix();
    glTranslatef(game->hscorebox->pos[0], game->hscorebox->pos[1], 0);
    glBindTexture(GL_TEXTURE_2D, game->hscorebox->hscoreboxTexture[0]);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2i(-wid,-height);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2i(-wid, height);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2i( wid, height);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2i( wid,-height);
    glEnd();
    glPopMatrix();
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);

    Rect r;
    r.bot = game->hscorebox->pos[1];
	r.left = game->hscorebox->pos[0];
	int xpos = game->hscorebox->pos[0]-getMidpoint(game);
	int ypos = game->hscorebox->pos[1]+25;
	glColor3i(1,1,0);

	drawScore(game->tempscore,game,20,xpos,ypos);
	r.bot = game->hscorebox->pos[1]-80;
	r.left = game->hscorebox->pos[0];
	ggprint40 (&r,50,0,"%s", &game->playername);
}

// remove the character | from name
void removebar(Game *game)
{
    game->playername[strlen(game->playername) - 1] = '\0';
}

void addbar(Game *game)
{
    strcat(game->playername, "|");
}

void resetName(Game *game)
{
    memset(game->playername, 0 , sizeof(game->playername));
}

void getName(XEvent *e, Game *game)
{
    if (strlen(game->playername)<11)
        removebar(game);
    int key;
    // check for return key before other keys
    if ( e->type == KeyPress ) {
		key = XLookupKeysym ( &e->xkey, 0 );
		switch ( key ) {
            case XK_BackSpace:
                if (strlen(game->playername)==11) {
                    game->playername[strlen(game->playername)-1] = '\0';
                    return;
                }
                break;
            case XK_Return:
                // if nothing entered, default = player
                if (strlen(game->playername)==11)
                    removebar(game);
                if (strlen(game->playername) == 0)
                strcat(game->playername, "player");
                game->isHighScore = false;
                // Kevin's function to write score to site
                sendScoresToPHP(game->playername, game->tempscore, game->difficulty);
                game->tempscore = 0;
                resetName(game);
                break;
		}
    }
    // max length is 8 characters
    if (strlen(game->playername) > 10)
        return;
    if ( e->type == KeyPress ) {
		key = XLookupKeysym ( &e->xkey, 0 );
		switch ( key ) {
            case XK_BackSpace:
                if (strlen(game->playername)>0 && strlen(game->playername)<=10 )
                    game->playername[strlen(game->playername)-1] = '\0';
                break;
			case XK_a:
				strcat(game->playername,"a");
				break;
            case XK_b:
				strcat(game->playername,"b");
				break;
			case XK_c:
				strcat(game->playername,"c");
				break;
            case XK_d:
				strcat(game->playername,"d");
				break;
            case XK_e:
				strcat(game->playername,"e");
				break;
            case XK_f:
				strcat(game->playername,"f");
				break;
            case XK_g:
				strcat(game->playername,"g");
				break;
            case XK_h:
				strcat(game->playername,"h");
				break;
            case XK_i:
				strcat(game->playername,"i");
				break;
            case XK_j:
				strcat(game->playername,"j");
				break;
            case XK_k:
				strcat(game->playername,"k");
				break;
			case XK_l:
				strcat(game->playername,"l");
				break;
			case XK_m:
                strcat(game->playername,"m");
				break;
			case XK_n:
                strcat(game->playername,"n");
				break;
			case XK_o:
                strcat(game->playername,"o");
				break;
			case XK_p:
                strcat(game->playername,"p");
				break;
            case XK_q:
                strcat(game->playername,"q");
				break;
			case XK_r:
                strcat(game->playername,"r");
				break;
            case XK_s:
                strcat(game->playername,"s");
				break;
            case XK_t:
                strcat(game->playername,"t");
				break;
            case XK_u:
                strcat(game->playername,"u");
				break;
			case XK_v:
                strcat(game->playername,"v");
				break;
            case XK_w:
                strcat(game->playername,"w");
				break;
            case XK_x:
                strcat(game->playername,"x");
				break;
            case XK_y:
                strcat(game->playername,"y");
				break;
            case XK_z:
                strcat(game->playername,"z");
				break;
            case XK_0:
                strcat(game->playername,"0");
				break;
            case XK_1:
                strcat(game->playername,"1");
				break;
            case XK_2:
                strcat(game->playername,"2");
				break;
            case XK_3:
                strcat(game->playername,"3");
				break;
			case XK_4:
                strcat(game->playername,"4");
				break;
            case XK_5:
                strcat(game->playername,"5");
				break;
            case XK_6:
                strcat(game->playername,"6");
				break;
            case XK_7:
                strcat(game->playername,"7");
				break;
            case XK_8:
                strcat(game->playername,"8");
				break;
            case XK_9:
                strcat(game->playername,"9");
				break;
            case XK_Return:
                game->isHighScore = false;
                game->tempscore = 0;
                resetName(game);
                break;
			case XK_Escape:
                game->isHighScore = false;
                resetName(game);
                game->tempscore = 0;
				break;
		}
	}
	addbar(game);
}

bool checkHighScore(Game *game, int s)
{
    if (game->score == 0) {
        return false;
    }
    int difficulty = game->difficulty;
    int dif;
    switch (difficulty) {
        case 1:
            dif = 1;
            break;
        case 2:
            dif = 2;
            break;
        case 3:
            dif = 3;
            break;
    }
    ifstream infile("hscore.txt");
    if (infile.fail()) {
        return false;
    }
    string score, mode;
    while (getline(infile, score, ',') && getline(infile, mode)) {
        int mtmp = atoi(mode.c_str());
        if (mtmp == dif) {
            string stmp = score;
            int _score = atoi(stmp.c_str());
            if (s > _score) {
                cout << "true";
                return true;
            }
        }
    }
    return false;
}

bool getHighScore(Game *game,char shost[],char spage[],bool cscore,bool pscore)
{
    struct sockaddr_in *remote;
    int sock;
    int tmpres;
    char *ip;
    char *get;
    char buf[BUFSIZ+1];
    char *host;
    char *page;

    host = new char[256];
    strcpy(host,shost);
    page = new char[256];
    strcpy(page, spage);
    sock = create_tcp_socket();
    ip = get_ip(host);
    remote = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in *));
    remote->sin_family = AF_INET;
    tmpres = inet_pton(AF_INET, ip, (void *)(&(remote->sin_addr.s_addr)));
    if (tmpres < 0) {
        perror("Can't set remote->sin_addr.s_addr");
        exit(1);
    } else if (tmpres == 0) {
        fprintf(stderr, "%s is not a valid IP address\n", ip);
        exit(1);
    }
    remote->sin_port = htons(PORT);

    if (connect(sock,(struct sockaddr *)remote,sizeof(struct sockaddr)) < 0) {
        perror("Could not connect");
        exit(1);
    }
    get = build_get_query(host, page);
    //Send the query to the server
    unsigned int sent = 0;
    while (sent < strlen(get)) {
        tmpres = send(sock, get+sent, strlen(get)-sent, 0);
        if (tmpres == -1) {
            perror("Can't send query");
            exit(1);
        }
        sent += tmpres;
    }
    //download highscore from server into text for processing
    if (cscore) {
        ofstream hscore("hscore.txt");
        memset(buf, 0, sizeof(buf));
        int htmlstart = 0;
        char * htmlcontent;
        while ((tmpres = recv(sock, buf, BUFSIZ, 0)) > 0) {
            if (htmlstart == 0) {
                htmlcontent = strstr(buf, "\r\n\r\n");
                if (htmlcontent != NULL) {
                    htmlstart = 1;
                    htmlcontent += 4;
                }
            } else {
                htmlcontent = buf;
            }
            if (htmlstart && cscore) {
                for (int i = 0; i < (int)strlen(htmlcontent); i++) {
                    hscore << htmlcontent[i];
                }
            }
            memset(buf, 0, tmpres);
        }
        hscore.close();
    }
    if (tmpres < 0) {
        perror("Error receiving data");
    }
    free(get);
    free(remote);
    free(ip);
    close(sock);
    return true;
}
