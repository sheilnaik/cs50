#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            int avg = round((float)(image[r][c].rgbtRed + image[r][c].rgbtGreen + image[r][c].rgbtBlue) / 3);

            image[r][c].rgbtRed = avg;
            image[r][c].rgbtGreen = avg;
            image[r][c].rgbtBlue = avg;
        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            int sepiaRed = round(.393 * image[r][c].rgbtRed + .769 * image[r][c].rgbtGreen + .189 * image[r][c].rgbtBlue);
            int sepiaGreen = round(.349 * image[r][c].rgbtRed + .686 * image[r][c].rgbtGreen + .168 * image[r][c].rgbtBlue);
            int sepiaBlue = round(.272 * image[r][c].rgbtRed + .534 * image[r][c].rgbtGreen + .131 * image[r][c].rgbtBlue);

            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }

            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }

            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            image[r][c].rgbtRed = sepiaRed;
            image[r][c].rgbtGreen = sepiaGreen;
            image[r][c].rgbtBlue = sepiaBlue;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE reflection[height][width];

    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            reflection[r][c].rgbtRed = image[r][width - c - 1].rgbtRed;
            reflection[r][c].rgbtBlue = image[r][width - c - 1].rgbtBlue;
            reflection[r][c].rgbtGreen = image[r][width - c - 1].rgbtGreen;
        }
    }

    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            image[r][c].rgbtRed = reflection[r][c].rgbtRed;
            image[r][c].rgbtBlue = reflection[r][c].rgbtBlue;
            image[r][c].rgbtGreen = reflection[r][c].rgbtGreen;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE blur[height][width];

    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            // Top-left +
            if (r == 0 && c == 0)
            {
                blur[r][c].rgbtRed = round((float)(image[r][c].rgbtRed + image[r][c + 1].rgbtRed + image[r + 1][c + 1].rgbtRed +
                                                   image[r + 1][c].rgbtRed) / 4);
                blur[r][c].rgbtBlue = round((float)(image[r][c].rgbtBlue + image[r][c + 1].rgbtBlue + image[r + 1][c + 1].rgbtBlue +
                                                    image[r + 1][c].rgbtBlue) / 4);
                blur[r][c].rgbtGreen = round((float)(image[r][c].rgbtGreen + image[r][c + 1].rgbtGreen + image[r + 1][c + 1].rgbtGreen +
                                                     image[r + 1][c].rgbtGreen) / 4);
            }
            // Top-right +
            else if (r == 0 && c == width - 1)
            {
                blur[r][c].rgbtRed = round((float)(image[r][c].rgbtRed + image[r][c - 1].rgbtRed + image[r + 1][c - 1].rgbtRed +
                                                   image[r + 1][c].rgbtRed) / 4);
                blur[r][c].rgbtBlue = round((float)(image[r][c].rgbtBlue + image[r][c - 1].rgbtBlue + image[r + 1][c - 1].rgbtBlue +
                                                    image[r + 1][c].rgbtBlue) / 4);
                blur[r][c].rgbtGreen = round((float)(image[r][c].rgbtGreen + image[r][c - 1].rgbtGreen + image[r + 1][c - 1].rgbtGreen +
                                                     image[r + 1][c].rgbtGreen) / 4);
            }
            // Bottom-left +
            else if (r == height - 1 && c == 0)
            {
                blur[r][c].rgbtRed = round((float)(image[r][c].rgbtRed + image[r - 1][c].rgbtRed + image[r - 1][c + 1].rgbtRed +
                                                   image[r][c + 1].rgbtRed) / 4);
                blur[r][c].rgbtBlue = round((float)(image[r][c].rgbtBlue + image[r - 1][c].rgbtBlue + image[r - 1][c + 1].rgbtBlue +
                                                    image[r][c + 1].rgbtBlue) / 4);
                blur[r][c].rgbtGreen = round((float)(image[r][c].rgbtGreen + image[r - 1][c].rgbtGreen + image[r - 1][c + 1].rgbtGreen +
                                                     image[r][c + 1].rgbtGreen) / 4);
            }
            // Bottom-right
            else if (r == height - 1 && c == width - 1)
            {
                blur[r][c].rgbtRed = round((float)(image[r][c].rgbtRed + image[r - 1][c].rgbtRed + image[r - 1][c - 1].rgbtRed +
                                                   image[r][c - 1].rgbtRed) / 4);
                blur[r][c].rgbtBlue = round((float)(image[r][c].rgbtBlue + image[r - 1][c].rgbtBlue + image[r - 1][c - 1].rgbtBlue +
                                                    image[r][c - 1].rgbtBlue) / 4);
                blur[r][c].rgbtGreen = round((float)(image[r][c].rgbtGreen + image[r - 1][c].rgbtGreen + image[r - 1][c - 1].rgbtGreen +
                                                     image[r][c - 1].rgbtGreen) / 4);
            }
            // Top edge
            else if (r == 0)
            {
                blur[r][c].rgbtRed = round((float)(image[r][c].rgbtRed + image[r][c - 1].rgbtRed + image[r + 1][c - 1].rgbtRed +
                                                   image[r + 1][c].rgbtRed + image[r + 1][c + 1].rgbtRed + image[r][c + 1].rgbtRed) / 6);
                blur[r][c].rgbtBlue = round((float)(image[r][c].rgbtBlue + image[r][c - 1].rgbtBlue + image[r + 1][c - 1].rgbtBlue +
                                                    image[r + 1][c].rgbtBlue + image[r + 1][c + 1].rgbtBlue + image[r][c + 1].rgbtBlue) / 6);
                blur[r][c].rgbtGreen = round((float)(image[r][c].rgbtGreen + image[r][c - 1].rgbtGreen + image[r + 1][c - 1].rgbtGreen +
                                                     image[r + 1][c].rgbtGreen + image[r + 1][c + 1].rgbtGreen + image[r][c + 1].rgbtGreen) / 6);
            }
            // left edge
            else if (c == 0)
            {
                blur[r][c].rgbtRed = round((float)(image[r][c].rgbtRed + image[r - 1][c].rgbtRed + image[r - 1][c + 1].rgbtRed +
                                                   image[r][c + 1].rgbtRed + image[r + 1][c + 1].rgbtRed + image[r + 1][c].rgbtRed) / 6);
                blur[r][c].rgbtBlue = round((float)(image[r][c].rgbtBlue + image[r - 1][c].rgbtBlue + image[r - 1][c + 1].rgbtBlue +
                                                    image[r][c + 1].rgbtBlue + image[r + 1][c + 1].rgbtBlue + image[r + 1][c].rgbtBlue) / 6);
                blur[r][c].rgbtGreen = round((float)(image[r][c].rgbtGreen + image[r - 1][c].rgbtGreen + image[r - 1][c + 1].rgbtGreen +
                                                     image[r][c + 1].rgbtGreen + image[r + 1][c + 1].rgbtGreen + image[r + 1][c].rgbtGreen) / 6);
            }
            // Bottom edge
            else if (r == height - 1)
            {
                blur[r][c].rgbtRed = round((float)(image[r][c].rgbtRed + image[r][c - 1].rgbtRed + image[r - 1][c - 1].rgbtRed +
                                                   image[r - 1][c].rgbtRed + image[r - 1][c + 1].rgbtRed + image[r][c + 1].rgbtRed) / 6);
                blur[r][c].rgbtBlue = round((float)(image[r][c].rgbtBlue + image[r][c - 1].rgbtBlue + image[r - 1][c - 1].rgbtBlue +
                                                    image[r - 1][c].rgbtBlue + image[r - 1][c + 1].rgbtBlue + image[r][c + 1].rgbtBlue) / 6);
                blur[r][c].rgbtGreen = round((float)(image[r][c].rgbtGreen + image[r][c - 1].rgbtGreen + image[r - 1][c - 1].rgbtGreen +
                                                     image[r - 1][c].rgbtGreen + image[r - 1][c + 1].rgbtGreen + image[r][c + 1].rgbtGreen) / 6);
            }
            // Right edge
            else if (c == width - 1)
            {
                blur[r][c].rgbtRed = round((float)(image[r][c].rgbtRed + image[r - 1][c].rgbtRed + image[r - 1][c - 1].rgbtRed +
                                                   image[r][c - 1].rgbtRed + image[r + 1][c - 1].rgbtRed + image[r + 1][c].rgbtRed) / 6);
                blur[r][c].rgbtBlue = round((float)(image[r][c].rgbtBlue + image[r - 1][c].rgbtBlue + image[r - 1][c - 1].rgbtBlue +
                                                    image[r][c - 1].rgbtBlue + image[r + 1][c - 1].rgbtBlue + image[r + 1][c].rgbtBlue) / 6);
                blur[r][c].rgbtGreen = round((float)(image[r][c].rgbtGreen + image[r - 1][c].rgbtGreen + image[r - 1][c - 1].rgbtGreen +
                                                     image[r][c - 1].rgbtGreen + image[r + 1][c - 1].rgbtGreen + image[r + 1][c].rgbtGreen) / 6);
            }
            else
            {
                blur[r][c].rgbtRed = round((float)(image[r][c].rgbtRed + image[r - 1][c - 1].rgbtRed + image[r - 1][c].rgbtRed +
                                                   image[r - 1][c + 1].rgbtRed + image[r][c + 1].rgbtRed + image[r + 1][c + 1].rgbtRed +
                                                   image[r + 1][c].rgbtRed + image[r + 1][c - 1].rgbtRed + image[r][c - 1].rgbtRed) / 9);
                blur[r][c].rgbtBlue = round((float)(image[r][c].rgbtBlue + image[r - 1][c - 1].rgbtBlue + image[r - 1][c].rgbtBlue +
                                                    image[r - 1][c + 1].rgbtBlue + image[r][c + 1].rgbtBlue + image[r + 1][c + 1].rgbtBlue +
                                                    image[r + 1][c].rgbtBlue + image[r + 1][c - 1].rgbtBlue + image[r][c - 1].rgbtBlue) / 9);
                blur[r][c].rgbtGreen = round((float)(image[r][c].rgbtGreen + image[r - 1][c - 1].rgbtGreen + image[r - 1][c].rgbtGreen +
                                                     image[r - 1][c + 1].rgbtGreen + image[r][c + 1].rgbtGreen + image[r + 1][c + 1].rgbtGreen +
                                                     image[r + 1][c].rgbtGreen + image[r + 1][c - 1].rgbtGreen + image[r][c - 1].rgbtGreen) / 9);
            }



        }
    }

    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            image[r][c].rgbtRed = blur[r][c].rgbtRed;
            image[r][c].rgbtBlue = blur[r][c].rgbtBlue;
            image[r][c].rgbtGreen = blur[r][c].rgbtGreen;
        }
    }

    return;
}
