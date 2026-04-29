#include"header.h"

static void wake_up_all(t_simulation *manger)
{
    int i;

    i = 0;
    while (i < manger->args.num_coders)
    {
        pthread_cond_broadcast(&manger->dongles[i].wake_dongle);
        i++;
    }
    
}

static int check_burnout(t_simulation *manger, int i)
{
    if(manger->coders[i].done == 0 && get_time_ms() - manger->coders[i].last_compile > manger->args.time_to_burnout)
    {
        pthread_mutex_unlock(&manger->pause);
        pthread_mutex_lock(&manger->pause_print);
        fprintf(stdout, "%s%ld %d burned out%s\n", BURNOUT,
			get_time_ms() - manger->coders[i].sim->start_time,
			manger->coders[i].id, RESET);
        pthread_mutex_unlock(&manger->pause_print);
        pthread_mutex_lock(&manger->pause);
        manger->simulation_running = 0;
        pthread_mutex_unlock(&manger->pause);
        wake_up_all(manger);
        return (1);
    }
    return (0);
}

static int check_coders(t_simulation *manger)
{
    int i;
    int round;

    i = 0;
    round = 0;
    while(i < manger->args.num_coders)
    {
        pthread_mutex_lock(&manger->pause);
        if(check_burnout(manger, i))
            return (-1);
        if (manger->coders[i].done == 1)
            round++;
        pthread_mutex_unlock(&manger->pause);
        i++;
    }
    return (round);
}

void	*monitor(void *arg)
{
	t_simulation	*manger;
	int round;

	manger = (t_simulation *)arg;
	pthread_mutex_lock(&manger->pause);
	while (manger->simulation_running == 1)
	{
		pthread_mutex_unlock(&manger->pause);
		usleep(1000);
        round = check_coders(manger);
        if (round == -1)
            return (NULL);
        if (round == manger->args.num_coders)
        {
            pthread_mutex_lock(&manger->pause);
            manger->simulation_running = 0;
            pthread_mutex_unlock(&manger->pause);
            wake_up_all(manger);
            return (NULL);
        }
        pthread_mutex_lock(&manger->pause);
    }
	pthread_mutex_unlock(&manger->pause);
	return (NULL);
}