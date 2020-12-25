#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <random>
#include <time.h>
#include <math.h>
#include <windows.h>

#define ITERATIONS 100

struct point
{
    float x;
    float y;
    float z;
};

int rand_range(int a, int b)
{
    return rand() % (b-a+1) + a;
}

float distance(point &p1, point &p2)
{
    float dist = (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z);
    return sqrtf(dist);
}

void closest_center(point* points, int npoints, point* centers, int k, int* clusters)
{
    for (int i = 0; i < npoints; i++)
    {
        float min = 150 * 150 * 150;
        int cluster = 0;
        for (int j = 0; j < k; j++)
        {
            float d = distance(points[i], centers[j]);
            if (d < min)
            {
                cluster = j;
                min = d;
            }
        }
        clusters[i] = cluster;
    }
}

int* clustering(point* points, int npoints, int k, point* centers)
{
    int* clusters = new int[npoints];

    for (int iter = 0; iter < ITERATIONS; iter++)
    {
        // Определение кластеров
        closest_center(points, npoints, centers, k, clusters);
        
        // Обнуляем значения координат центров
        for (int i = 0; i < k; i++)
        {
            centers[i].x = 0;
            centers[i].y = 0;
            centers[i].z = 0;
        }

        // Суммируем значение центра
        for (int i = 0; i < npoints; i++)
        {
            centers[clusters[i]].x = centers[clusters[i]].x + points[i].x;
            centers[clusters[i]].y = centers[clusters[i]].y + points[i].y;
            centers[clusters[i]].z = centers[clusters[i]].z + points[i].z;
        }

        // Усредняем
        for (int i = 0; i < k; i++)
        {
            int cl = 0;
            // Считаем длину кластера
            for (int j = 0; j < npoints; j++)
                if (clusters[j] == i)
                    cl = cl + 1;

            if (cl)
            {
                centers[i].x = centers[i].x / float(cl);
                centers[i].y = centers[i].y / float(cl);
                centers[i].z = centers[i].z / float(cl);
            }
        }
    }
    
    return clusters;
}

int *clustering_opt(point *points, int npoints, int k, point *centers)
{
    int *clusters = new int[npoints];

    for (int iter = 0; iter < ITERATIONS; iter++)
    {
        // Определение кластеров
        closest_center(points, npoints, centers, k, clusters);
        
        // Рассчитываем новые центры
        for (int i = 0; i < k; i++)
        {
            // Обнуляем
            centers[i].x = 0;
            centers[i].y = 0;
            centers[i].z = 0;

            // Подсчитываем длину и суммируем значение
            int cl = 0;
            for (int j = 0; j < npoints; j++)
            {
                if (clusters[j] == i)
                {
                    cl++;
                    centers[i].x += points[j].x;
                    centers[i].y += points[j].y;
                    centers[i].z += points[j].z;
                }
            }
            if (cl)
            {
                centers[i].x /= float(cl);
                centers[i].y /= float(cl);
                centers[i].z /= float(cl);
            }
        }
    }

    return clusters;
}

void generate_points(point* arr, int n)
{
    for (int i = 0; i < n; i++)
    {   
        arr[i].x = rand_range(-100, 100);
        arr[i].y = rand_range(-100, 100);
        arr[i].z = rand_range(-100, 100);
    }
}

int main()
{
    srand(time(NULL));

    int n = 10000;
    point* points = new point[n];
    generate_points(points, n);

    int k = 50;
    point* centers = new point[k];
    point *centers2 = new point[k];
    // Инициализируем центры случайными значениями
    for (int i = 0; i < k; i++)
    {
        centers[i].x = rand_range(-100, 100);
        centers[i].y = rand_range(-100, 100);
        centers[i].z = rand_range(-100, 100);
        centers2[i].x = centers[i].x;
        centers2[i].y = centers[i].y;
        centers2[i].z = centers[i].z;
    }

    StartCounter();
    int* clusters = clustering(points, n, k, centers);
    double elapsed_time = GetCounter();
    printf("Cluster numbers unoptimized: ");
    for (int i = 0; i < n; i++)
        std::cout << clusters[i] << " ";
    std::cout << "\n";
    for (int i = 0; i < k; i++)
        printf("Centroid#%d: %7.3f %7.3f %7.3f\n", i, centers[i].x, centers[i].y, centers[i].z);
    delete[] clusters;

    StartCounter();
    clusters = clustering_opt(points, n, k, centers2);
    elapsed_time = GetCounter();
    printf("Cluster numbers optimized: ");
    for (int i = 0; i < n; i++)
        std::cout << clusters[i] << " ";
    std::cout << "\n";
    for (int i = 0; i < k; i++)
        printf("Centroid#%d: %7.3f %7.3f %7.3f\n", i, centers2[i].x, centers2[i].y, centers2[i].z);

    delete []points;
    delete []clusters;
    return 0;
}
