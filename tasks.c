#include "header.h"

int	is_complete(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->pause);
	if (coder->sim->simulation_running == 0 || coder->done == 1)
	{
		pthread_mutex_unlock(&coder->sim->pause);
		return (1);
	}
	pthread_mutex_unlock(&coder->sim->pause);
	return (0);
}

void	log_line(t_coder *coder, char *message)
{
	pthread_mutex_lock(&coder->sim->pause_print);
	if (strcmp(message, "burned out") == 0 || is_complete(coder) == 1)
	{
		if (strcmp(message, "burned out") == 0)
		{
			fprintf(stdout, "%s%ld %d %s %s\n", coder->color, get_time_ms()
				- coder->sim->start_time, coder->id, message, RESET);
		}
		pthread_mutex_unlock(&coder->sim->pause_print);
		return ;
	}
	fprintf(stdout, "%s%ld %d %s %s\n", coder->color, get_time_ms()
		- coder->sim->start_time, coder->id, message, RESET);
	pthread_mutex_unlock(&coder->sim->pause_print);
}

void	compile(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->pause_print);
	pthread_mutex_lock(&coder->sim->pause);
	if (coder->sim->simulation_running == 0 || coder->done == 1)
	{
		pthread_mutex_unlock(&coder->sim->pause);
		pthread_mutex_unlock(&coder->sim->pause_print);
		return ;
	}
	pthread_mutex_unlock(&coder->sim->pause);
	fprintf(stdout, "%s%ld %d is compiling%s\n", coder->color, get_time_ms()
		- coder->sim->start_time, coder->id, RESET);
	pthread_mutex_unlock(&coder->sim->pause_print);
	pthread_mutex_lock(&coder->sim->pause);
	coder->last_compile = get_time_ms();
	coder->number_of_compilations++;
	pthread_mutex_unlock(&coder->sim->pause);
	usleep(coder->sim->args.time_to_compile * 1000);
	release_dongle(coder->right_dongle);
	release_dongle(coder->left_dongle);
}

void	debuging(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->pause_print);
	pthread_mutex_lock(&coder->sim->pause);
	if (coder->sim->simulation_running == 0)
	{
		pthread_mutex_unlock(&coder->sim->pause);
		pthread_mutex_unlock(&coder->sim->pause_print);
		return ;
	}
	pthread_mutex_unlock(&coder->sim->pause);
	fprintf(stdout, "%s%ld %d is debugging%s\n", coder->color, get_time_ms()
		- coder->sim->start_time, coder->id, RESET);
	pthread_mutex_unlock(&coder->sim->pause_print);
	usleep(coder->sim->args.time_to_debug * 1000);
}

void	refactoring(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->pause_print);
	pthread_mutex_lock(&coder->sim->pause);
	if (coder->sim->simulation_running == 0)
	{
		pthread_mutex_unlock(&coder->sim->pause);
		pthread_mutex_unlock(&coder->sim->pause_print);
		return ;
	}
	pthread_mutex_unlock(&coder->sim->pause);
	fprintf(stdout, "%s%ld %d is refactoring%s\n", coder->color, get_time_ms()
		- coder->sim->start_time, coder->id, RESET);
	pthread_mutex_unlock(&coder->sim->pause_print);
	usleep(coder->sim->args.time_to_refactor * 1000);
	pthread_mutex_lock(&coder->sim->pause);
	if (coder->number_of_compilations == coder->sim->args.num_compiles_required)
		coder->done = 1;
	pthread_mutex_unlock(&coder->sim->pause);
}
