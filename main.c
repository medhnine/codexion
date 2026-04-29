#include "header.h"

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->pause_dongle);
	dongle->release = get_time_ms();
	dongle->is_taken = 0;
	pthread_cond_broadcast(&dongle->wake_dongle);
	pthread_mutex_unlock(&dongle->pause_dongle);
}

void	*simulation(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	pthread_mutex_lock(&coder->sim->pause);
	while (coder->sim->simulation_running == 1)
	{
		pthread_mutex_unlock(&coder->sim->pause);
		if (coder->id % 2 == 0)
		{
			take_dongle(coder, coder->right_dongle);
			take_dongle(coder, coder->left_dongle);
		}
		else
		{
			take_dongle(coder, coder->left_dongle);
			take_dongle(coder, coder->right_dongle);
		}
		compile(coder);
		debuging(coder);
		refactoring(coder);
		pthread_mutex_lock(&coder->sim->pause);
		if (coder->sim->simulation_running == 0)
		{
			pthread_mutex_unlock(&coder->sim->pause);
			return (NULL);
		}
	}
	pthread_mutex_unlock(&coder->sim->pause);
	return (NULL);
}

int main(int ac, char **argv)
{
    t_args argument;
    t_coder *list_coders;
    t_simulation get;
    pthread_t manger;

    if (ac != 9)
    {
        fprintf(stderr, "the number of arguments is not 9");
        return 1;
    }
    if (parse_arg(argv, &argument) == 1)
        return 1;

    list_coders = malloc(sizeof(t_coder) * argument.num_coders);
    get.start_time = get_time_ms();
    get.dongles = malloc(sizeof(t_dongle) * argument.num_coders);
    get.args = argument;
    get.coders = list_coders;
    get.simulation_running = 1;
    pthread_mutex_init(&get.pause, NULL);
    pthread_mutex_init(&get.pause_print, NULL);
    pthread_cond_init(&get.wake_up, NULL);
	intalaize_thredas(argument, list_coders, &get);
	pthread_create(&manger, NULL, &monitor, (void *)&get);
	threads_manger(&get, argument, list_coders);
    pthread_join(manger, NULL);
    return 0;
}