#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Define a struct for vector operations
struct Vector
{
    double x;
    double y;
    double z;
    double magnitude;
    double direction;
    char description[200]; // Increase the description size
    // Exclude some members from usage
    // int unusedField1;
    // double unusedField2;
    // Add more fields as needed
};

// Define a struct for matrix operations
struct Matrix
{
    double elements[6][6]; // Increase the matrix size
    double determinant;
    double trace;
    // Exclude some members from usage
    // double unusedField3;
    // Add more fields as needed
};

// Define a struct for complex number operations
struct ComplexNumber
{
    double real;
    double imaginary;
    double magnitude;
    double phase;
    // Exclude some members from usage
    // char unusedField4[100];
    // Add more fields as needed
};

// Define a struct for representing particles
struct Particle
{
    int id;
    double mass;
    struct Vector position;
    struct Vector velocity;
    // Add more fields as needed
};

// Define a struct for sensor data
struct SensorData
{
    int sensorId;
    double value;
    char timestamp[30];
    // Add more fields as needed
};

int main()
{
    // Seed for random values
    srand(time(NULL));

    // Define arrays of struct instances to store data
    struct Vector vectors[3];
    struct Matrix matrices[2];
    struct ComplexNumber complexNumbers[4];
    struct Particle particles[3];
    struct SensorData sensorData[5];

    // Manual member assignment for vectors
    for (int i = 0; i < 3; ++i)
    {
        vectors[i].x = rand() % 10 + 1; // Random values for x
        vectors[i].y = rand() % 10 + 1; // Random values for y
        vectors[i].z = rand() % 10 + 1; // Random values for z
        snprintf(vectors[i].description, sizeof(vectors[i].description), "Vector %d description", i + 1);
    }

    // Manual member assignment for matrices
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 6; ++j)
        {
            for (int k = 0; k < 6; ++k)
            {
                matrices[i].elements[j][k] = rand() % 10 + 1; // Random values for matrix elements
            }
        }
    }

    // Manual member assignment for complex numbers
    for (int i = 0; i < 4; ++i)
    {
        complexNumbers[i].real = rand() % 10 + 1;      // Random values for real part
        complexNumbers[i].imaginary = rand() % 10 + 1; // Random values for imaginary part
    }

    // Manual member assignment for particles
    for (int i = 0; i < 3; ++i)
    {
        particles[i].id = i + 1;
        particles[i].mass = rand() % 20 + 1;       // Random values for mass
        particles[i].position.x = rand() % 10 + 1; // Random values for x position
        particles[i].position.y = rand() % 10 + 1; // Random values for y position
        particles[i].position.z = rand() % 10 + 1; // Random values for z position
        particles[i].velocity.x = rand() % 5 + 1;  // Random values for x velocity
        particles[i].velocity.y = rand() % 5 + 1;  // Random values for y velocity
        particles[i].velocity.z = rand() % 5 + 1;  // Random values for z velocity
    }

    // Manual member assignment for sensor data
    for (int i = 0; i < 5; ++i)
    {
        sensorData[i].sensorId = i + 1;
        sensorData[i].value = (rand() % 1000) / 10.0; // Random values for sensor value
        snprintf(sensorData[i].timestamp, sizeof(sensorData[i].timestamp), "2023-12-01 12:%02d:%02d", rand() % 60,
                 rand() % 60);
    }

    // Loop with more than 100 iterations
    for (int iteration = 0; iteration < 150; ++iteration)
    {
        // Perform mathematical operations on vectors
        for (int i = 0; i < 3; ++i)
        {
            vectors[i].magnitude = sqrt(vectors[i].x * vectors[i].x +
                                        vectors[i].y * vectors[i].y +
                                        vectors[i].z * vectors[i].z);

            vectors[i].direction = atan2(vectors[i].y, vectors[i].x);
        }

        // Perform mathematical operations on matrices
        for (int i = 0; i < 2; ++i)
        {
            // Calculate determinant
            matrices[i].determinant =
                matrices[i].elements[0][0] * (matrices[i].elements[1][1] * matrices[i].elements[2][2] -
                                              matrices[i].elements[2][1] * matrices[i].elements[1][2]) -
                matrices[i].elements[0][1] * (matrices[i].elements[1][0] * matrices[i].elements[2][2] -
                                              matrices[i].elements[2][0] * matrices[i].elements[1][2]) +
                matrices[i].elements[0][2] * (matrices[i].elements[1][0] * matrices[i].elements[2][1] -
                                              matrices[i].elements[2][0] * matrices[i].elements[1][1]);

            // Calculate trace
            matrices[i].trace = matrices[i].elements[0][0] + matrices[i].elements[1][1] + matrices[i].elements[2][2];
        }

        // Perform mathematical operations on complex numbers
        for (int i = 0; i < 4; ++i)
        {
            complexNumbers[i].magnitude = sqrt(complexNumbers[i].real * complexNumbers[i].real +
                                               complexNumbers[i].imaginary * complexNumbers[i].imaginary);

            complexNumbers[i].phase = atan2(complexNumbers[i].imaginary, complexNumbers[i].real);
        }

        // Randomly update particle positions and velocities
        for (int i = 0; i < 3; ++i)
        {
            particles[i].position.x += (rand() % 5) - 2; // Random update for x position
            particles[i].position.y += (rand() % 5) - 2; // Random update for y position
            particles[i].position.z += (rand() % 5) - 2; // Random update for z position
            particles[i].velocity.x += (rand() % 3) - 1; // Random update for x velocity
            particles[i].velocity.y += (rand() % 3) - 1; // Random update for y velocity
            particles[i].velocity.z += (rand() % 3) - 1; // Random update for z velocity
        }

        // Randomly update sensor values
        for (int i = 0; i < 5; ++i)
        {
            sensorData[i].value += ((rand() % 200) - 100) / 10.0; // Random update for sensor value
        }

        // Display some information after every 50 iterations
        if (iteration % 50 == 0)
        {
            printf("\nIteration %d:\n", iteration);
            // Display some information from each struct type
            printf("Vectors:\n");
            for (int i = 0; i < 3; ++i)
            {
                printf("Vector %d: Magnitude=%.2f, Direction=%.2f radians, Description=%s\n", i + 1,
                       vectors[i].magnitude, vectors[i].direction, vectors[i].description);
            }

            printf("\nMatrices:\n");
            for (int i = 0; i < 2; ++i)
            {
                printf("Matrix %d: Determinant=%.2f, Trace=%.2f\n", i + 1, matrices[i].determinant, matrices[i].trace);
            }

            printf("\nComplex Numbers:\n");
            for (int i = 0; i < 4; ++i)
            {
                printf("Complex Number %d: Magnitude=%.2f, Phase=%.2f radians\n", i + 1, complexNumbers[i].magnitude,
                       complexNumbers[i].phase);
            }

            printf("\nParticles:\n");
            for (int i = 0; i < 3; ++i)
            {
                printf("Particle %d: ID=%d, Mass=%.2f, Position=%.2f, %.2f, %.2f, Velocity=%.2f, %.2f, %.2f\n", i + 1,
                       particles[i].id, particles[i].mass, particles[i].position.x, particles[i].position.y,
                       particles[i].position.z, particles[i].velocity.x, particles[i].velocity.y,
                       particles[i].velocity.z);
            }

            printf("\nSensor Data:\n");
            for (int i = 0; i < 5; ++i)
            {
                printf("Sensor %d: ID=%d, Value=%.2f, Timestamp=%s\n", i + 1, sensorData[i].sensorId,
                       sensorData[i].value, sensorData[i].timestamp);
            }
        }
    }

    return 0;
}
