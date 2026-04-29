# ifndef HEADER_H
# define HEADER_H

#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

# define RESET   "\033[0m"
# define BURNOUT "\033[1;31m"

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
    int done;
    pthread_t thread;
    t_dongle *left_dongle;
    t_dongle *right_dongle;
    long last_compile;
    int number_of_compilations;
    t_simulation *sim;
    char *color;
} t_coder;

int	ft_atoi(char *str);
int parse_arg(char **av, t_args *info);
char *get_color(int id);
void	compile(t_coder *coder);
void	debuging(t_coder *coder);
void	refactoring(t_coder *coder);
long	get_time_ms(void);
void	insert_heap(t_dongle *dongle, t_edf info);
void	insert_down(t_dongle *dongle);
t_edf	pop_heap(t_dongle *dongle);
void	release_dongle(t_dongle *dongle);
void	clean_up(void *target);
void    intalaize_thredas(t_args argument, t_coder *list_coders, t_simulation *get);
void	*simulation(void *arg);
void    threads_manger(t_simulation *get, t_args argument, t_coder *list_coders);
void	*monitor(void *arg);
void	*monitor(void *arg);
void	take_dongle(t_coder *coder, t_dongle *dongle);
void log_line(t_coder *coder, char *message);
int is_complete(t_coder *coder);

# endif