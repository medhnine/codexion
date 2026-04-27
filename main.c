#include "header.h"

long get_time_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void insert_heap(t_dongle *dongle, t_edf info)
{
    t_edf temp;
    int i, head;

    dongle->quee[dongle->size] = info;
    dongle->size++;
    i = dongle->size - 1;
    while (i > 0)
    {
        head = (i - 1) / 2;
        if(dongle->quee[i].deadline >= dongle->quee[head].deadline)
            break;
        temp = dongle->quee[i];
        dongle->quee[i] = dongle->quee[head];
        dongle->quee[head] = temp;
        i = head;
    }
}

t_edf pop_heap(t_dongle *dongle)
{
    t_edf temp;

    temp = dongle->quee[0];
    dongle->size--;
    dongle->quee[0] = dongle->quee[dongle->size];
    return temp;
}

void release_dongle(t_dongle *dongle)
{
    pthread_mutex_lock(&dongle->pause_dongle);
    dongle->release = get_time_ms();
    dongle->is_taken = 0;
    pthread_cond_broadcast(&dongle->wake_dongle);
    pthread_mutex_unlock(&dongle->pause_dongle);
}
 
void take_dongle(t_coder *coder, t_dongle *dongle)
{
    long total_ms;
    t_edf hold;
    struct timespec ts;
    pthread_mutex_lock(&dongle->pause_dongle);
    hold.id = coder->id;
    if (coder->sim->args.scheduler == 0)
        hold.deadline = get_time_ms();
    else
        hold.deadline = coder->last_compile + coder->sim->args.time_to_burnout;
    insert_heap(dongle, hold);
    while (dongle->is_taken == 1 ||
         (get_time_ms() - dongle->release < coder->sim->args.dongle_cooldown) || 
        dongle->quee[0].id != coder->id)
    {
        total_ms = get_time_ms() + 1;
        ts.tv_sec  = total_ms / 1000;
        ts.tv_nsec = (total_ms % 1000) * 1000000;
        if (coder->sim->simulation_running == 0)
        {
            dongle->size--;
            pthread_mutex_unlock(&dongle->pause_dongle);
            return;
        }
        pthread_cond_timedwait(&dongle->wake_dongle, &dongle->pause_dongle, &ts);
    }
    pthread_mutex_lock(&coder->sim->pause);
    if (coder->sim->simulation_running == 0)
    {
        dongle->size--;
        pthread_mutex_unlock(&coder->sim->pause);
        pthread_mutex_unlock(&dongle->pause_dongle);
        pthread_cond_broadcast(&dongle->wake_dongle);
        return;
    }
    pthread_mutex_unlock(&coder->sim->pause);
    dongle->is_taken = 1;
    fprintf(stdout, "%ld %d has taken a dongle\n", get_time_ms() - coder->sim->start_time, coder->id);
    dongle->quee[0] = dongle->quee[1];
    dongle->size--;
    pthread_cond_broadcast(&dongle->wake_dongle);
    pthread_mutex_unlock(&dongle->pause_dongle);
}

void *simulation(void *arg)
{
    t_coder *coder;
    
    coder = (t_coder *)arg;
    pthread_mutex_lock(&coder->sim->pause);
    while (coder->sim->simulation_running == 1)
    {
        pthread_mutex_unlock(&coder->sim->pause);
        if (coder->id == 1) {
            take_dongle(coder, coder->right_dongle);
            take_dongle(coder, coder->left_dongle);
        }
        else {
            take_dongle(coder, coder->left_dongle);
            take_dongle(coder, coder->right_dongle);
        }
        pthread_mutex_lock(&coder->sim->pause);
        if (coder->sim->simulation_running == 0)
        {
            pthread_mutex_unlock(&coder->sim->pause);
            return (NULL);
        }
        pthread_mutex_unlock(&coder->sim->pause);
        pthread_mutex_lock(&coder->sim->pause);
        coder->last_compile = get_time_ms();
        pthread_mutex_unlock(&coder->sim->pause);
        pthread_mutex_lock(&coder->sim->pause_print);
        fprintf(stdout, "%ld %d is compiling\n", get_time_ms() - coder->sim->start_time, coder->id);
        pthread_mutex_unlock(&coder->sim->pause_print);
        usleep(coder->sim->args.time_to_compile * 1000);
        release_dongle(coder->right_dongle);
        release_dongle(coder->left_dongle);
        pthread_mutex_lock(&coder->sim->pause);
        coder->number_of_compilations++;
        pthread_mutex_unlock(&coder->sim->pause);
        pthread_mutex_lock(&coder->sim->pause);
        if (coder->sim->simulation_running == 0)
        {
            pthread_mutex_unlock(&coder->sim->pause);
            return (NULL);
        }
        pthread_mutex_unlock(&coder->sim->pause);
        pthread_mutex_lock(&coder->sim->pause_print);
        fprintf(stdout, "%ld %d is debugging\n", get_time_ms() - coder->sim->start_time, coder->id);
        pthread_mutex_unlock(&coder->sim->pause_print);
        usleep(coder->sim->args.time_to_debug * 1000);
        pthread_mutex_lock(&coder->sim->pause);
        if (coder->sim->simulation_running == 0)
        {
            pthread_mutex_unlock(&coder->sim->pause);
            return (NULL);
        }
        pthread_mutex_unlock(&coder->sim->pause);
        pthread_mutex_lock(&coder->sim->pause_print);
        fprintf(stdout, "%ld %d is refactoring\n", get_time_ms() - coder->sim->start_time, coder->id);
        pthread_mutex_unlock(&coder->sim->pause_print);
        usleep(coder->sim->args.time_to_refactor * 1000);
        pthread_mutex_lock(&coder->sim->pause);
        if (coder->sim->simulation_running == 0)
        {
            pthread_mutex_unlock(&coder->sim->pause);
            return (NULL);
        }
        pthread_mutex_unlock(&coder->sim->pause);
        pthread_mutex_lock(&coder->sim->pause);
    }
    pthread_mutex_unlock(&coder->sim->pause);
    return NULL;
}

void *monitor(void *arg)
{
    t_simulation *manger;
    int i, num_of_comp, x;

    manger = (t_simulation *)arg;
    i = 0;
    pthread_mutex_lock(&manger->pause);
    while (manger->simulation_running == 1)
    {
        x = 0;
        pthread_mutex_unlock(&manger->pause);
        num_of_comp = 0;
        i = 0;
        usleep(1000);
        while (i < manger->args.num_coders)
        {
            pthread_mutex_lock(&manger->pause);
            if(get_time_ms() - manger->coders[i].last_compile > manger->args.time_to_burnout)
            {
                pthread_mutex_unlock(&manger->pause);
                pthread_mutex_lock(&manger->pause_print);
                fprintf(stdout, "%ld %d burned out\n", get_time_ms() - manger->coders[i].sim->start_time, manger->coders[i].id);
                pthread_mutex_unlock(&manger->pause_print);
                pthread_mutex_lock(&manger->pause);
                manger->simulation_running = 0;
                pthread_mutex_unlock(&manger->pause);
                while (x < manger->args.num_coders)
                {
                    pthread_mutex_lock(&manger->dongles[x].pause_dongle);
                    pthread_cond_broadcast(&manger->dongles[x].wake_dongle);
                    pthread_mutex_unlock(&manger->dongles[x].pause_dongle);
                    x++;
                }
                break;
            }
            if (manger->coders[i].number_of_compilations >= manger->args.num_compiles_required)
                num_of_comp++;
            pthread_mutex_unlock(&manger->pause);
            i++;
        }
        if(num_of_comp == manger->args.num_coders)
        {
            manger->simulation_running = 0;
            x = 0;
            while (x < manger->args.num_coders)
            {
                pthread_cond_broadcast(&manger->dongles[x].wake_dongle);
                x++;
            }
            pthread_mutex_unlock(&manger->pause);
            break;
        }
        pthread_mutex_lock(&manger->pause);
    }
    pthread_mutex_unlock(&manger->pause);
    return (NULL);
}

void clean_up(void *target)
{
    free(target);
    target = NULL;
}

int main(int ac, char **argv)
{
    t_args argument;
    t_coder *list_coders;
    t_simulation get;
    pthread_t manger;
    int i;

    if (ac != 9)
    {
        fprintf(stderr, "the number of arguments is not 9");
        return 1;
    }
    if (parse_arg(argv, &argument) == 1)
        return 1;
    
    i = 0;
    list_coders = malloc(sizeof(t_coder) * argument.num_coders);
    get.start_time = get_time_ms();
    get.dongles = malloc(sizeof(t_dongle) * argument.num_coders);
    get.args = argument;
    get.coders = list_coders;
    get.simulation_running = 1;
    pthread_mutex_init(&get.pause, NULL);
    pthread_mutex_init(&get.pause_print, NULL);
    pthread_cond_init(&get.wake_up, NULL);

    while (i < argument.num_coders)
    {
        list_coders[i].id = i + 1;
        list_coders[i].number_of_compilations = 0;
        list_coders[i].last_compile = get.start_time;
        pthread_mutex_init(&get.dongles[i].pause_dongle, NULL);
        pthread_cond_init(&get.dongles[i].wake_dongle, NULL);
        get.dongles[i].release = 0;
        get.dongles[i].size = 0;
        get.dongles[i].is_taken = 0;
        list_coders[i].left_dongle = &get.dongles[i];
        list_coders[i].right_dongle = &get.dongles[(i + 1) % argument.num_coders];
        list_coders[i].sim = &get;
        i++;
    }
    i = 0;
    while (i < argument.num_coders)
    {
        pthread_create(&list_coders[i].thread, NULL, &simulation, (void *)&list_coders[i]);
        i++;
    }
    pthread_create(&manger, NULL, &monitor, (void *)&get);
    i = 0;
    while (i < argument.num_coders)
    {
        pthread_join(list_coders[i].thread, NULL);
        pthread_mutex_destroy(&get.dongles[i].pause_dongle);
        pthread_cond_destroy(&get.dongles[i].wake_dongle);
        i++;
    }
    pthread_join(manger, NULL);
    free(list_coders);
    free(get.dongles);
    pthread_mutex_destroy(&get.pause);
    pthread_mutex_destroy(&get.pause_print);
    pthread_cond_destroy(&get.wake_up);
    return 0;
}