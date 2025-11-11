#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define WIDTH 64
#define HEIGHT 64
#define FPS 30

typedef struct {
    int frame_id;
    int width, height;
    unsigned char *data;
    struct timespec ts;
} Frame;

//Node 1: Frame Generator
Frame *generate_frame(int id) {
    Frame *f = malloc(sizeof(Frame));
    f->frame_id = id;
    f->width = WIDTH;
    f->height = HEIGHT;
    f->data = malloc(WIDTH * HEIGHT);

    for (int i = 0; i < WIDTH * HEIGHT; i++)
        f->data[i] = rand() % 256;

    clock_gettime(CLOCK_REALTIME, &f->ts);
    return f;
}

// Node 2: Frame Processing
void invert_frame(Frame *f) {
    for (int i = 0; i < f->width * f->height; i++)
        f->data[i] = 255 - f->data[i];
}

void adjust_brightness(Frame *f, int delta) {
    for (int i = 0; i < f->width * f->height; i++) {
        int val = f->data[i] + delta;
        if (val < 0) val = 0;
        if (val > 255) val = 255;
        f->data[i] = val;
    }
}

void threshold(Frame *f, unsigned char th) {
    for (int i = 0; i < f->width * f->height; i++)
        f->data[i] = (f->data[i] > th) ? 255 : 0;
}

// Node 3: Display / Logger
void display_frame_info(Frame *f, const char *stage, const char *effect, double proc_time) {
    printf("[%s] Frame %02d | Time: %ld.%09ld | Pixel[0]=%3d",
           stage, f->frame_id, f->ts.tv_sec, f->ts.tv_nsec, f->data[0]);
    if (effect)
        printf(" | Effect: %-10s | ProcTime: %.6f s", effect, proc_time);
    printf("\n");
}

// Main Simulation
int main() {
    srand(time(NULL));
    int frame_count = 10;
    int choice;
    printf("\n=== RT-VidOS: Toy Real-Time Video Processor ===\n");
    printf("Choose Processing Effect:\n");
    printf("1) Invert\n2) Brightness Adjust\n3) Threshold\n> ");
    scanf("%d", &choice);

    int delta = 0, th = 128;
    if (choice == 2) {
        printf("Enter brightness delta (-100 to +100): ");
        scanf("%d", &delta);
    } else if (choice == 3) {
        printf("Enter threshold (0-255): ");
        scanf("%d", &th);
    }

    for (int i = 0; i < frame_count; i++) {
        Frame *f = generate_frame(i);
        display_frame_info(f, "Generated", NULL, 0.0);

        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        // Apply chosen effect
        switch (choice) {
            case 1: invert_frame(f); break;
            case 2: adjust_brightness(f, delta); break;
            case 3: threshold(f, th); break;
            default: invert_frame(f); break;
        }

        clock_gettime(CLOCK_MONOTONIC, &end);
        double elapsed = (end.tv_sec - start.tv_sec) +
                         (end.tv_nsec - start.tv_nsec) / 1e9;

        display_frame_info(f, "Processed",
                           (choice == 1) ? "Invert" :
                           (choice == 2) ? "Brightness" :
                           (choice == 3) ? "Threshold" : "Invert",
                           elapsed);
        printf("------------------------------------------------------\n");

        usleep(1000000 / FPS);  // maintain 30 FPS simulation
        free(f->data);
        free(f);
    }

    printf("\nSimulation complete. Ready for Review 1!\n");
    return 0;
}
