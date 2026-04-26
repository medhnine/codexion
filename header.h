# ifndef HEADER_H
# define HEADER_H

#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>



typedef struct s_coder t_coder;
typedef struct s_simulation t_simulation;

typedef struct s_args
{
    int    num_coders;
    int    time_to_burnout;
    int    time_to_compile;
    int    time_to_debug;
    int    time_to_refactor;
    int    num_compiles_required;
    int    dongle_cooldown;
    int    scheduler;
}   t_args;

typedef struct s_edf
{
    int id;
    long deadline;

} t_edf;

typedef struct s_dongle
{
    pthread_mutex_t pause_dongle;
    pthread_cond_t wake_dongle;
    t_edf quee[2];
    int size;
    int is_taken;
    long release;
} t_dongle;


typedef struct s_simulation
{
    pthread_mutex_t pause_print;
    t_args args;
    t_coder *coders;
    t_dongle *dongles;
    pthread_mutex_t pause;
    int simulation_running;
    long start_time;
    pthread_cond_t wake_up;
} t_simulation;


typedef struct s_coder
{
    int id;
    pthread_t thread;
    t_dongle *left_dongle;
    t_dongle *right_dongle;
    long last_compile;
    int number_of_compilations;
    t_simulation *sim;
} t_coder;

int	ft_atoi(char *str);
int parse_arg(char **av, t_args *info);

# endif