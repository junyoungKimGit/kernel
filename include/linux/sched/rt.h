#ifndef _SCHED_RT_H
#define _SCHED_RT_H

#include <linux/sched/prio.h>


/* IAMROOT-12:
 * -------------
 * RT 스케쥴러가 사용하는 priority 값이면 true를 반환한다.
 */
static inline int rt_prio(int prio)
{
	if (unlikely(prio < MAX_RT_PRIO))
		return 1;
	return 0;
}

/* IAMROOT-12:
 * -------------
 * RT 태스크 여부를 priority 값으로 확인한다.
 */
static inline int rt_task(struct task_struct *p)
{
	return rt_prio(p->prio);
}

#ifdef CONFIG_RT_MUTEXES
extern int rt_mutex_getprio(struct task_struct *p);
extern void rt_mutex_setprio(struct task_struct *p, int prio);
extern int rt_mutex_get_effective_prio(struct task_struct *task, int newprio);
extern struct task_struct *rt_mutex_get_top_task(struct task_struct *task);
extern void rt_mutex_adjust_pi(struct task_struct *p);
static inline bool tsk_is_pi_blocked(struct task_struct *tsk)
{
	return tsk->pi_blocked_on != NULL;
}
#else
static inline int rt_mutex_getprio(struct task_struct *p)
{
	return p->normal_prio;
}

static inline int rt_mutex_get_effective_prio(struct task_struct *task,
					      int newprio)
{
	return newprio;
}

static inline struct task_struct *rt_mutex_get_top_task(struct task_struct *task)
{
	return NULL;
}
# define rt_mutex_adjust_pi(p)		do { } while (0)
static inline bool tsk_is_pi_blocked(struct task_struct *tsk)
{
	return false;
}
#endif

extern void normalize_rt_tasks(void);


/*
 * default timeslice is 100 msecs (used only for SCHED_RR tasks).
 * Timeslices get refilled after they expire.
 */

/* IAMROOT-12:
 * -------------
 * 라운드 로빈 rt 태스크의 주기
 */
#define RR_TIMESLICE		(100 * HZ / 1000)

#endif /* _SCHED_RT_H */
