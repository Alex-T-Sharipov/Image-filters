#include "helpers.h"
#include <math.h>

int min(int a, int b);
int round1(float a);

int avgclr_blue(int a, int height, int width, int i, int j, RGBTRIPLE image[height][width]);
int avgclr_green(int a, int height, int width, int i, int j, RGBTRIPLE image[height][width]);
int avgclr_red(int a, int height, int width, int i, int j, RGBTRIPLE image[height][width]);

int edge_blue(int height, int width, int i, int j, RGBTRIPLE image[height][width]);
int edge_green(int height, int width, int i, int j, RGBTRIPLE image[height][width]);
int edge_red(int height, int width, int i, int j, RGBTRIPLE image[height][width]);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            int b = image[i][j].rgbtBlue;
            int g = image[i][j].rgbtGreen;
            int r = image[i][j].rgbtRed;

            int a = round1( (b+g+r)/3.0 );

            image[i][j].rgbtBlue = min(a, 255);
            image[i][j].rgbtGreen = min(a, 255);
            image[i][j].rgbtRed = min(a, 255);
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            int b = image[i][j].rgbtBlue;
            int g = image[i][j].rgbtGreen;
            int r = image[i][j].rgbtRed;

            int rs = round1( .393 * r + .769 * g + .189 * b );
            int gs = round1(.349 * r + .686 * g + .168 * b);
            int bs = round1(.272 * r + .534 * g + .131 * b);

            image[i][j].rgbtBlue = min(bs, 255);
            image[i][j].rgbtGreen = min(gs, 255);
            image[i][j].rgbtRed = min(rs, 255);
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < round1(width/2.0); j++)
        {
            int b2 = image[i][width-1-j].rgbtBlue;
            int g2 = image[i][width-1-j].rgbtGreen;
            int r2 = image[i][width-1-j].rgbtRed;

            int b1 = image[i][j].rgbtBlue;
            int g1 = image[i][j].rgbtGreen;
            int r1 = image[i][j].rgbtRed;

            image[i][j].rgbtBlue = b2;
            image[i][j].rgbtGreen = g2;
            image[i][j].rgbtRed = r2;

            image[i][width-1-j].rgbtBlue = b1;
            image[i][width-1-j].rgbtGreen = g1;
            image[i][width-1-j].rgbtRed = r1;

        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE array[height][width];

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            int b = image[i][j].rgbtBlue;
            b = avgclr_blue(b, height, width, i, j, image);
            array[i][j].rgbtBlue = b;

            int g = image[i][j].rgbtGreen;
            g = avgclr_green(g, height, width, i, j, image);
            array[i][j].rgbtGreen = g;
            
            int r = image[i][j].rgbtRed;
            r = avgclr_red(r, height, width, i, j, image);
            array[i][j].rgbtRed = r;
        }
    }
    
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = array[i][j].rgbtBlue;
            image[i][j].rgbtGreen = array[i][j].rgbtGreen;
            image[i][j].rgbtRed = array[i][j].rgbtRed;
        }
    }

    return;
}


// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    
    RGBTRIPLE array[height][width];

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            int b = image[i][j].rgbtBlue;
            array[i][j].rgbtBlue = edge_blue(height, width, i, j, image);

            int g = image[i][j].rgbtGreen;
            array[i][j].rgbtGreen = edge_green(height, width, i, j, image);
            
            int r = image[i][j].rgbtRed;
            array[i][j].rgbtRed = edge_red(height, width, i, j, image);
        }
    }
    
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = array[i][j].rgbtBlue;
            image[i][j].rgbtGreen = array[i][j].rgbtGreen;
            image[i][j].rgbtRed = array[i][j].rgbtRed;
        }
    }
    
    return;
}


int min(int a, int b)
{
    if(a <= b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

int round1(float a)
{
    if(a - floor(a) >= 0.5)
    {
        return floor(a) + 1;
    }
    else
    {
        return floor(a);
    }
}

int avgclr_blue(int a, int height, int width, int i, int j, RGBTRIPLE image[height][width])
{
    int b = a;
    float counter = 1.0;
    if(i - 1 >= 0)
    {
        b += image[i-1][j].rgbtBlue;
        counter++;
        if(j-1 >= 0)
        {
            b += image[i-1][j-1].rgbtBlue;
            counter++;
        }
        if(j+1 < width)
        {
            b += image[i-1][j+1].rgbtBlue;
            counter++;
        }

    }
    if(i + 1 < height)
    {
        b += image[i+1][j].rgbtBlue;
        counter++;
        if(j+1 < width)
        {
            b += image[i+1][j+1].rgbtBlue;
            counter++;
        }
        if(j-1 >= 0)
        {
            b += image[i+1][j-1].rgbtBlue;
            counter++;
        }
    }

    if(j - 1 >= 0)
    {
        b += image[i][j-1].rgbtBlue;
        counter++;
    }
    if(j + 1 < width)
    {
        b += image[i][j+1].rgbtBlue;
        counter++;
    }
    return min(round1(b / counter), 255);
}

int avgclr_green(int a, int height, int width, int i, int j, RGBTRIPLE image[height][width])
{
    int b = a;
    float counter = 1.0;
    if(i - 1 >= 0)
    {
        b += image[i-1][j].rgbtGreen;
        counter++;
        if(j-1 >= 0)
        {
            b += image[i-1][j-1].rgbtGreen;
            counter++;
        }
        if(j+1 < width)
        {
            b += image[i-1][j+1].rgbtGreen;
            counter++;
        }

    }
    if(i + 1 < height)
    {
        b += image[i+1][j].rgbtGreen;
        counter++;
        if(j+1 < width)
        {
            b += image[i+1][j+1].rgbtGreen;
            counter++;
        }
        if(j-1 >= 0)
        {
            b += image[i+1][j-1].rgbtGreen;
            counter++;
        }
    }

    if(j - 1 >= 0)
    {
        b += image[i][j-1].rgbtGreen;
        counter++;
    }
    if(j + 1 < width)
    {
        b += image[i][j+1].rgbtGreen;
        counter++;
    }
    return min(round1(b / counter), 255);
}

int avgclr_red(int a, int height, int width, int i, int j, RGBTRIPLE image[height][width])
{
    int b = a;
    float counter = 1.0;
    if(i - 1 >= 0)
    {
        b += image[i-1][j].rgbtRed;
        counter++;
        if(j-1 >= 0)
        {
            b += image[i-1][j-1].rgbtRed;
            counter++;
        }
        if(j+1 < width)
        {
            b += image[i-1][j+1].rgbtRed;
            counter++;
        }

    }
    if(i + 1 < height)
    {
        b += image[i+1][j].rgbtRed;
        counter++;
        if(j+1 < width)
        {
            b += image[i+1][j+1].rgbtRed;
            counter++;
        }
        if(j-1 >= 0)
        {
            b += image[i+1][j-1].rgbtRed;
            counter++;
        }
    }

    if(j - 1 >= 0)
    {
        b += image[i][j-1].rgbtRed;
        counter++;
    }
    if(j + 1 < width)
    {
        b += image[i][j+1].rgbtRed;
        counter++;
    }
    return min(round1(b / counter), 255);
}

int edge_red(int height, int width, int i, int j, RGBTRIPLE image[height][width])
{
    
    int gx = 0;
    int gy = 0;
    
    if(i - 1 >= 0)
    {
        gy += image[i-1][j].rgbtRed * (-2);

        if(j-1 >= 0)
        {
            gy += image[i-1][j-1].rgbtRed * (-1);
            gx += image[i-1][j-1].rgbtRed * (-1);

        }
        if(j+1 < width)
        {
            gy += image[i-1][j+1].rgbtRed * (-1);
            gx += image[i-1][j+1].rgbtRed * (1);

        }

    }
    if(i + 1 < height)
    {
        gy += image[i+1][j].rgbtRed * (2);

        if(j+1 < width)
        {
            gy += image[i+1][j+1].rgbtRed * (1);
            gx += image[i+1][j+1].rgbtRed * (1);

        }
        if(j-1 >= 0)
        {
            gy += image[i+1][j-1].rgbtRed * (1);
            gx += image[i+1][j-1].rgbtRed * (-1);

        }
    }

    if(j - 1 >= 0)
    {
        gx += image[i][j-1].rgbtRed * (-2);

    }
    if(j + 1 < width)
    {
        gx += image[i][j+1].rgbtRed * (+2);

    }
    
    return( min( round1( sqrt(gx*gx + gy*gy) ), 255 ) );
}

int edge_green(int height, int width, int i, int j, RGBTRIPLE image[height][width])
{
    
    int gx = 0;
    int gy = 0;
    
    if(i - 1 >= 0)
    {
        gy += image[i-1][j].rgbtGreen * (-2);

        if(j-1 >= 0)
        {
            gy += image[i-1][j-1].rgbtGreen * (-1);
            gx += image[i-1][j-1].rgbtGreen * (-1);

        }
        if(j+1 < width)
        {
            gy += image[i-1][j+1].rgbtGreen * (-1);
            gx += image[i-1][j+1].rgbtGreen * (1);

        }

    }
    if(i + 1 < height)
    {
        gy += image[i+1][j].rgbtGreen * (2);

        if(j+1 < width)
        {
            gy += image[i+1][j+1].rgbtGreen * (1);
            gx += image[i+1][j+1].rgbtGreen * (1);

        }
        if(j-1 >= 0)
        {
            gy += image[i+1][j-1].rgbtGreen * (1);
            gx += image[i+1][j-1].rgbtGreen * (-1);

        }
    }

    if(j - 1 >= 0)
    {
        gx += image[i][j-1].rgbtGreen * (-2);

    }
    if(j + 1 < width)
    {
        gx += image[i][j+1].rgbtGreen * (+2);

    }
    
    return( min( round1( sqrt(gx*gx + gy*gy) ), 255 ) );
}

int edge_blue(int height, int width, int i, int j, RGBTRIPLE image[height][width])
{
    
    int gx = 0;
    int gy = 0;
    
    if(i - 1 >= 0)
    {
        gy += image[i-1][j].rgbtBlue * (-2);

        if(j-1 >= 0)
        {
            gy += image[i-1][j-1].rgbtBlue * (-1);
            gx += image[i-1][j-1].rgbtBlue * (-1);

        }
        if(j+1 < width)
        {
            gy += image[i-1][j+1].rgbtBlue * (-1);
            gx += image[i-1][j+1].rgbtBlue * (1);

        }

    }
    if(i + 1 < height)
    {
        gy += image[i+1][j].rgbtBlue * (2);

        if(j+1 < width)
        {
            gy += image[i+1][j+1].rgbtBlue * (1);
            gx += image[i+1][j+1].rgbtBlue * (1);

        }
        if(j-1 >= 0)
        {
            gy += image[i+1][j-1].rgbtBlue * (1);
            gx += image[i+1][j-1].rgbtBlue * (-1);

        }
    }

    if(j - 1 >= 0)
    {
        gx += image[i][j-1].rgbtBlue * (-2);

    }
    if(j + 1 < width)
    {
        gx += image[i][j+1].rgbtBlue * (+2);

    }
    
    return( min( round1( sqrt(gx*gx + gy*gy) ), 255 ) );
}