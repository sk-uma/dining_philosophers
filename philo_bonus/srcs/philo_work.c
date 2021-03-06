#include "philo_three.h"

static int	philo_take_fork(t_philo *philo)
{
	if (sem_wait(philo->fork_sem))
		return (error_log(ERROR_SEM_WAIT));
	philo_log(philo->philo_nb, LOG_TAKEEN, philo->print_bsem);
	if (sem_wait(philo->fork_sem))
		return (error_log(ERROR_SEM_WAIT));
	philo_log(philo->philo_nb, LOG_TAKEEN, philo->print_bsem);
	return (0);
}

static int	philo_put_fork(t_philo *philo)
{
	if (sem_post(philo->fork_sem))
		return (error_log(ERROR_SEM_POST));
	if (sem_post(philo->fork_sem))
		return (error_log(ERROR_SEM_POST));
	return (0);
}

static int	philo_after_eat(t_philo *philo)
{
	philo_log(philo->philo_nb, LOG_EATING, philo->print_bsem);
	if (sem_wait(philo->let_bsem))
		return (error_log(ERROR_SEM_WAIT));
	philo->last_eat_time = get_time();
	if (sem_post(philo->let_bsem))
		return (error_log(ERROR_SEM_POST));
	if (sem_post(philo->eat_count))
		return (error_log(ERROR_SEM_POST));
	wait_time(philo->time_to_eat);
	if (philo_put_fork(philo))
		return (1);
	philo_log(philo->philo_nb, LOG_SLEEPING, philo->print_bsem);
	wait_time(philo->time_to_sleep);
	philo_log(philo->philo_nb, LOG_THINKING, philo->print_bsem);
	return (0);
}

void	*philo_work(void *arg)
{
	t_philo	*philo;
	int		ret;

	philo = (t_philo *)arg;
	if (sem_wait(philo->let_bsem))
		return (NULL);
	philo->last_eat_time = get_time();
	if (sem_post(philo->let_bsem))
		return (NULL);
	if (philo->philo_nb % 2)
		usleep(200);
	ret = 0;
	while (!ret)
	{
		philo_take_fork(philo);
		ret = philo_after_eat(philo);
	}
	return (NULL);
}
