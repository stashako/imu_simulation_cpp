#include <iostream>
#include <unistd.h>
#include <vector>
#include "i2c_multiplexer.h"
#include "getbno055.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

using namespace std;

char senaddr_[256] = "0x70";
char i2c_bus[256] = I2CBUS;
char senaddr[256] = "0x28";

class i2cmultiplexer
{

public:
    i2cmultiplexer()
    {
        get_i2cbus_multiplexer(i2c_bus, senaddr_);
    }

    void switch_channel(multiplexer_t channel)
    {
        set_multiplexer(channel);
    }
};

class imuSensor
{

public:
    imuSensor()
    {
        get_i2cbus(i2c_bus, senaddr);
    }
    void mode(opmode_t newmode)
    {
        //set the sensor operational mode
        set_mode(newmode);
        //newmode = acconly;
        //newmode = magonly;
        //newmode = gyronly;
        //newmode = accmag;
        //newmode = accgyro;
        //newmode = maggyro;
        //newmode = amg;
        //newmode = imu;
        //newmode = compass;
        //newmode = m4g;
        //newmode = ndof;
        //newmode = ndof_fmc;
    }

    void power(power_t newmode1)
    {
        //set the sensor power mode
        set_power(newmode1);
        //newmode1 = normal;
        //newmode1 = low;
        //newmode1 = suspend;
    }

    int *imu_data()
    {
        static int imu_data[22];

        struct bnoacc bnod1;
        get_acc(&bnod1);
        imu_data[0] = bnod1.adata_x;
        imu_data[1] = bnod1.adata_y;
        imu_data[2] = bnod1.adata_z;

        struct bnomag bnod2;
        get_mag(&bnod2);
        imu_data[3] = bnod2.mdata_x;
        imu_data[4] = bnod2.mdata_y;
        imu_data[5] = bnod2.mdata_z;

        struct bnogyr bnod3;
        get_gyr(&bnod3);
        imu_data[6] = bnod3.gdata_x;
        imu_data[7] = bnod3.gdata_y;
        imu_data[8] = bnod3.gdata_z;

        struct bnoeul bnod4;
        get_eul(&bnod4);
        imu_data[9] = bnod4.eul_head;
        imu_data[10] = bnod4.eul_roll;
        imu_data[11] = bnod4.eul_pitc;

        struct bnoqua bnod5;
        get_qua(&bnod5);
        imu_data[12] = bnod5.quater_w;
        imu_data[13] = bnod5.quater_x;
        imu_data[14] = bnod5.quater_y;
        imu_data[15] = bnod5.quater_z;

        struct bnogra bnod6;
        get_gra(&bnod6);
        imu_data[16] = bnod6.gravityx;
        imu_data[17] = bnod6.gravityy;
        imu_data[18] = bnod6.gravityz;

        struct bnolin bnod7;
        get_lin(&bnod7);
        imu_data[19] = bnod7.linacc_x;
        imu_data[20] = bnod7.linacc_y;
        imu_data[21] = bnod7.linacc_z;

        return imu_data;
    }
};

void render();
void init();
void reshape(int w, int h);
void timer(int);

float shift = -8.0;
float roll = 0.0;
float pitch = 0.0;
float yaw = 0.0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
    i2cmultiplexer multi;

    vector<multiplexer_t> ch = {channel1, channel2, channel3, channel4, channel5, channel6};

    imuSensor imu;
    imu.mode(ndof_fmc);

    multi.switch_channel(channel3);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowPosition(100, 100);
    glutInitWindowSize(500, 500);

    glutCreateWindow("Crawler");

    glutDisplayFunc(render);
    glutReshapeFunc(reshape);
    init();

    glutTimerFunc(0, timer, 0);

    glutMainLoop();

    // while (true)
    // {
    //     imuData = imu.imu_data();

    //     cout << "Euler angles: ";

    //     for (int i = 9; i < 12; i++)
    //     {
    //         cout << *(imuData + i) << " ";
    //     }
    //     cout << endl;

    //     sleep(1);
    // }

    return 0;
}

void init()
{

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0, 0.0, shift);
    glRotatef(roll, 1, 0, 0);
    glRotatef(pitch, 0, 1, 0);
    glRotatef(yaw, 0, 0, 1);

    glBegin(GL_QUADS);

    //front
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);
    //back
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(1.0, 1.0, -1.0);
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0, 1.0, -1.0);
    //right
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(1.0, 1.0, -1.0);
    //left
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(-1.0, 1.0, -1.0);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(-1.0, 1.0, 1.0);
    //top
    glColor3f(0.0, 1.0, 1.0);
    glVertex3f(-1.0, 1.0, -1.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(1.0, 1.0, -1.0);
    //bottom
    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, -1.0);

    glEnd();

    glutSwapBuffers();
}

void reshape(int w, int h)
{

    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 2.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);
    
    imuSensor imu;
    imu.mode(ndof_fmc);

    int *imuData;

    imuData = imu.imu_data();

    roll = *(imuData + 9);
    pitch = *(imuData + 10);
    yaw = *(imuData + 11);

    cout << endl;
}