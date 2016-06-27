/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/28 21:50:54 by cchaumar          #+#    #+#             */
/*   Updated: 2016/04/29 21:05:42 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# include "rt.h"
# define PARSER_H
# define CONTAINER		1
# define EMBEDDER		0
# define GRAMMAR_FILE	"grammar/grammar.gr"
# define INTEGRITY_FILE	"integrity/integrity.txt"
# define RESERVED		" .-+xyz:"
# define NUM_OBJS		5
# define AVAILABLE_TAGS	{"scene", "sphere", "plane", "point", "cone", "cylinder", "position", "color", "position", "direction", "borders", "type", "light", "texture", "composite", NULL}

typedef struct	s_objt
{
	char	*obj_name;
	int		type;
}				t_objt;

typedef struct	s_vec3f
{
	float	x;
	float	y;
	float	z;
}				t_vec3f;

typedef	struct	s_tags
{
	struct s_tags	*next;
	struct s_tags	*prev;
	struct s_tags	*tags;
	char			*tag;
	char			*attribute;
	char			*content_text;
	t_vec3f			*content;
	int				type;
}				t_tags;

typedef struct	s_gram
{
	char	*global;
	t_objt	*objs;
}				t_gram;

typedef struct	s_validator
{
	char	*tag;
	int		times;
	int		saved;
	int		present;
}				t_validator;

typedef struct	s_integrity
{
	char		*original;
	t_validator	*needed;
	t_validator	*optional;
}				t_integrity;

int		find_tag(char *src, char *tag);
int		find_closing_tag(char *src, char *tag);
t_tags	*list_tags(char *src);
char	*ft_exterminate(char *src);

void	ft_errors_denom_check(t_vec3f *boolean, char **truc);
int		ft_check_denom_assign(char **truc, t_vec3f *boolean);
float	ft_ftoa(char *str);

int		ft_tablen(char **str);
void	check_validity(t_tags *tags, t_integrity *integrity_matrix);
void	create_integrity_matrix(t_integrity **integrity);
int		find_next_symbol(char *src, int offset, char symbol);
int		find_attributes(char *src, int offset, int len_tagat, t_tags *tag);
void	type_giver_checker(t_tags *tags);
int		ft_check_vect(char *str, t_tags *tags);
void	vec3f_from_text(t_tags *tags);
char	*get_corresponding_attribute_tag(char *config_file, char *tag);
void	ft_little_if_2(int boole, t_tags *tags);
void	tags_attribute_checker(t_tags *tags);
void	fill_content(t_tags *tags);
void	check_src_exist(char **src);
void	check_needed_valid(char **buff_0, int i);
t_validator	*get_validator(char **buff_2, char **buff_1);
t_integrity	*parse_integrity(char **src, int i);
int			retard_way_of_getting_number_of_lines(int i, int fd, char *buff);
void		create_integrity_matrix(t_integrity **integrity);
void		validity_creator_opt(int i, int a, t_integrity *i_m, t_tags *tags);
int			contain_valid(t_tags *tags, t_integrity *i_m);
void		validity_creator_need(int i, int a, t_integrity *i_m, t_tags *tags);
int			contain_min(t_tags *tags, t_integrity *i_m);
int		integrity_check(t_tags *tags, t_integrity *i_m);
int		reality_check(char *tag);
void	check_validity(t_tags *tags, t_integrity *integrity_matrix);
t_tags	*tag_init(void);
t_tags	*tag_next_init(t_tags *tag_prev);
void	die_nofree(char *msg);
void	content_filler(t_tags **tags);
void	little_if(t_tags **tags, int *i);
void	do_next(t_tags **tags, int *i, int len_0);
void	tags_rewind(t_tags **tags);
t_tags	*list_tags(char *src);
int		find_tag(char *src, char *tag);
int		find_closing_tag(char *src, char *tag);
int		count_without_spaces(char *src);
char	*ft_exterminate(char *src);
int		ft_check_color(char *s);
void	color_from_text(t_tags *tags);

#endif
