#include "liste.h"

int	list_size(t_list *begin)
{
  int	i;

  i = 0;
  while (begin != NULL)
    {
      begin = begin->next;
      i = i + 1;
    }
  return (i);
}

void		rm_from_list(t_list **begin, t_list *trm, void (*f)(void*))
{
  while ((*begin))
    {
      if ((*begin) == trm)
	{
	  (*begin) = (*begin)->next;
	  if (f)
	    {
	      f(trm->data);
	      trm->data = NULL;
	    }
	  free(trm);
	  return ;
	}
      begin = &((*begin)->next);
    }
}

void		apply_on_list(t_list *begin,
                      int (*f)(void *, void *), void *arg)
{
  t_list	*list;

  list = begin;
  while (list != NULL)
    {
      (*f)(list->data, arg);
      list = list->next;
    }
}

void	rm_list(t_list *begin, void (*f)(void *))
{
  if (begin != NULL)
    {
      if (begin->next != NULL)
        rm_list(begin->next, f);
      if (f)
	(*f)(begin->data);
      free(begin);
    }
}

void		add_to_list(t_list **begin, void *data)
{
  t_list	*tmp;
  t_list	*new;

  if (!(new = malloc(sizeof(t_list))))
    return ;
  new->data = data;
  new->next = NULL;
  if (!(*begin) && (*begin = new))
    return ;
  tmp = *begin;
  while (tmp->next != NULL)
    tmp = tmp->next;
  tmp->next = new;
}
