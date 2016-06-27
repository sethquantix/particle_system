#include "parser.h"

t_tags	*tag_init(void)
{
	t_tags	*tags;

	tags = (t_tags*)malloc(sizeof(t_tags));
	tags->prev = NULL;
	tags->next = NULL;
	tags->tags = NULL;
	return (tags);
}

t_tags	*tag_next_init(t_tags *tag_prev)
{
	t_tags	*tags;

	tags = (t_tags*)malloc(sizeof(t_tags));
	tags->prev = tag_prev;
	tags->next = NULL;
	return (tags);
}

void	die_nofree(char *msg)
{
	ft_printf("%s\n", msg);
	exit(0);
}

void	content_filler(t_tags **tags)
{
	if ((*tags)->type == EMBEDDER)
		(*tags)->content = NULL;
	else
		fill_content(*tags);
}

void	little_if(t_tags **tags, int *i)
{
	if ((*tags)->content_text)
	{
		(*tags)->tags = list_tags((*tags)->content_text);
		*i += ft_strlen((*tags)->content_text);
	}
}
