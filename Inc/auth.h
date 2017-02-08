/*
 * auth.h
 *
 *  Created on: 2 de fev de 2017
 *      Author: Abdoral Neto, Adriano Pereira, Bruno Amaral
 */

#ifndef AUTH_H_
#define AUTH_H_

typedef enum {
	AU_FAIL = 0,			/* (0) Falha na autenticação ou no cadastro */
	AU_AUTHENTICATED,		/* (1) Usuário autenticado */
	AU_REGISTERED,	        /* (2) Usuário cadastrado */
	AU_DELETED	            /* (3) Usuário deletado */
} AUTHRESULT;


AUTHRESULT log(FIL* file, char *loginpass);

AUTHRESULT regist(FIL* file, char *login, char *pass);

AUTHRESULT delete(FIL* file, char *login, char *pass);


#endif /* AUTH_H_ */
