/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ameskine <ameskine@student.1337.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 23:42:15 by ameskine          #+#    #+#             */
/*   Updated: 2025/07/21 11:24:09 by ameskine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void foo1(char *s)
{
    int i = 0;
    while(s[i])
    {
        printf("%s", s[i]);
        i++;
    }
}


void *foo(void *arg)
{
    int i = 0;
    char *s = (char *)arg;
    while(s[i])
    {
        printf("%c", s[i]);
        i++;
    }
}

int main ()
{
    pthread_t new_thread;
    pthread_t new_thread1;
    char *s = "aminemeskine";
    pthread_create(&new_thread, NULL, foo, s);
    pthread_create(&new_thread1, NULL, foo, s);
    foo("jkfdsjgfjdhfd");
     pthread_join(new_thread, NULL);
     pthread_join(new_thread1, NULL);
}
