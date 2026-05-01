#include "header.h"

void	insert_heap(t_dongle *dongle, t_edf info)
{
	t_edf	temp;
	int		i;
	int		head;

	dongle->quee[dongle->size] = info;
	dongle->size++;
	i = dongle->size - 1;
	while (i > 0)
	{
		head = (i - 1) / 2;
		if (dongle->quee[i].deadline >= dongle->quee[head].deadline)
			break ;
		temp = dongle->quee[i];
		dongle->quee[i] = dongle->quee[head];
		dongle->quee[head] = temp;
		i = head;
	}
}

void	set_deadline(t_dongle *dongle, int index, long smallest)
{
	t_edf	temp;

	temp = dongle->quee[index];
	dongle->quee[index] = dongle->quee[smallest];
	dongle->quee[smallest] = temp;
	index = smallest;
}

void	insert_down(t_dongle *dongle)
{
	long	smallest;
	int		left;
	int		right;
	int		index;

	index = 0;
	while (1)
	{
		left = 2 * index + 1;
		right = 2 * index + 2;
		if (left >= dongle->size)
			break ;
		if (right < dongle->size
			&& dongle->quee[right].deadline < dongle->quee[left].deadline)
			smallest = right;
		else
			smallest = left;
		if (dongle->quee[index].deadline > dongle->quee[smallest].deadline)
			set_deadline(dongle, index, smallest);
		else
			break ;
	}
}

t_edf	pop_heap(t_dongle *dongle)
{
	t_edf	temp;

	temp = dongle->quee[0];
	dongle->size--;
	dongle->quee[0] = dongle->quee[dongle->size];
	insert_down(dongle);
	return (temp);
}
