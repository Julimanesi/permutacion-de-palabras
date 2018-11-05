#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#define NROPALABRAS 87596

#define TAM 25
unsigned int factorial(unsigned int n);
int cmp(const void *s1,const void *s2);
int bincmp(const void *s1,const void *s2);
int bin2cmp(const void *s1,const void *s2);
int bin3cmp(const void *s1,const void *s2);
int bin4cmp(const void *s1,const void *s2);
void generarcomb(int m, int n,char *palabra,char **com);
void generarPerm(char * A,char ** perm);
void swap(char *a,char * b);
int busquedaBin(char ** palabras,char *buscada,int size);
void strBarN(char *origen);//quita el enter

//void reemp_acento(char *palabra);
int main()
{	 
		char pBuscada[TAM];
		int i=0,j=0,k=0,n,cantPer,cantComb,cantCP;//cantPer: num de permutaciones resultantes, n:largo de los caracteres ingresados, cantComb: num de combinaciones resultantes
		int elem;//número de elementos (tamano de la palabra buscada)
		char ** pal_con_largo;//array con palabras del diccionario con un determinado largo
		int pal_con_size=0;//contador de palabras con el tamaño indicado en elem
		char ** perm;
		char ** comb;
		//Desde este punto guardo las palabras del diccionario en el array
		FILE * file = fopen("diccionario_rep.txt","r");//abro el archivo con todas las palabras del idioma y los
		if (file == NULL)
		{ 	
			printf("No se pudo abrir el diccionario\n");
			exit(-1);
		}
		char lista[NROPALABRAS][TAM];		    //verbos en infinitivo
		char * line=malloc(sizeof(char)*TAM);

		printf("Ingrese los caracteres:\n");
		scanf("%s",pBuscada);
		printf("Ingrese el número de elementos:\n");
		scanf("%d",&elem);
		//elem++;
		while ( fgets(line,TAM, file) !=NULL) {
			strBarN(line);//quito el enter
			if(i<NROPALABRAS)
		 	{	
				strncpy(lista[i], line,TAM);//guardo las palabras en el array
				if(strlen(line)==elem){pal_con_size++;}
			}
		 	i++;
	    }
	    fclose(file);
	 
	    //qsort(lista,NROPALABRAS,sizeof(char)*TAM,bincmp);//ordeno las palabras
		
		//AQUI TERMINO DE GUARDAR LAS PALABRAS Y ORDENARLAS
		pal_con_largo=(char **)malloc(sizeof(char *)*pal_con_size);
		for(i = 0; i < pal_con_size; i++)
		{	
			*(pal_con_largo+i)=(char *)malloc(sizeof(char)*elem);
		}
		int cuenta=0;
		for(i=0;i<NROPALABRAS;i++)
		{
			if(strlen(lista[i])==elem)
			{
				//printf("%s\n",lista[i]);
				strncpy(pal_con_largo[cuenta],lista[i],elem);
				cuenta++;
			}
		}
		qsort(pal_con_largo,pal_con_size,sizeof(pal_con_largo[0]),cmp);
		n =strlen(pBuscada);
		cantPer=factorial(elem);
		//perm[cant][n];
		perm=(char **)malloc(sizeof(char *)*cantPer);
		for(i = 0; i < cantPer; i++)
		{	
			*(perm+i)=(char *)malloc(sizeof(char)*elem);
		}
		if(perm == NULL)
		{
			printf("Ocurrio un error: La cantidad de permutaciones exede el limite\n");
			return -1;
		}
		cantComb=factorial(n)/(factorial(n-elem)*factorial(elem));
		printf("N:%d\n",n);
		printf("elem:%d\n",elem);
		printf("cantComb:%d\n",cantComb);
		printf("cantPer:%d\n",cantPer);
		comb=(char **)malloc(sizeof(char *)*cantComb);
		for(i = 0; i < cantComb; i++)
		{	
			*(comb+i)=(char *)malloc(sizeof(char)*(elem));
		}
		generarcomb(n,elem,pBuscada,comb);
		if(strlen(comb[0])>elem)
		{
			printf("Ocurrio un error: La cantidad de combinaciones exede el limite\n");
			return -1;
		}
		cantCP=cantComb*cantPer;
		printf("cantCP:%d\n",cantCP);
		printf("Las combinaciones ocupan:%d bytes\n",sizeof(char *)*cantComb+sizeof(char)*(elem)*cantComb);
		printf("Las permutaciones ocupan:%d bytes\n",sizeof(char *)*cantPer+sizeof(char)*(elem)*cantPer);
		printf("tamanio necesario en memoria: %d bytes\n",sizeof(char *)*cantComb+sizeof(char)*(elem)*cantComb+sizeof(char *)*cantPer+sizeof(char)*(elem)*cantPer);
		printf("Tiempo estimado :%lf segundos\n",(double)cantCP*2.5/1600000);//1600000 = Hz 9=cant de operaciones
		k=0;
		int cont=0;
		int sizeResult=20;
		char ** result=(char **)malloc(sizeof(char *)*sizeResult);
		for(i=0;i<sizeResult;i++)
		{
			result[i]=(char *)malloc(sizeof(char)*elem);
		}
		for(i=0;i<cantComb;i++)
		{
			//printf("%s\n",comb[i]);
			generarPerm(comb[i],perm);
			for(j=0;j<cantPer;j++)
			{
				
					if(busquedaBin(pal_con_largo,perm[j],pal_con_size))
					{	
						//printf("La palabra  \"%s\" EXISTE. \n",perm[j]);
						if(!contiene(result,perm[j],k))
						{	
							strncpy(result[k],perm[j],elem+1);//copia los resultados
							k++;//aumenta el tamaño del array 
							if(k>=sizeResult)
							{
								sizeResult=sizeResult+sizeResult/2;
								result=(char **)realloc(result,sizeof(char *)*sizeResult);
								for(cont=k;cont<sizeResult;cont++)
									result[cont]=(char *)malloc(sizeof(char)*elem);
							}
						}
					}
		 	 fflush(stdin);
			}
		}
		for(i=0;i<cantPer;i++)
		{
			free(*(perm+i));
		}
		free(perm);
		for(i=0;i<cantComb;i++)
		{
			free(*(comb+i));
		}
		free(comb);
		sizeResult=k;
		result=(char **)realloc(result,sizeof(char *)*sizeResult);
		printf("Cantidad de resultados:%d\n",sizeResult);
		qsort(result,sizeResult,sizeof(result[0]),cmp);//ordeno las palabras
		for(i=0;i<sizeResult;i++)
		{
				printf("La palabra %d: \"%s\" EXISTE\n",i,result[i]);
		}
		/*for(i=1;i<sizeResult;i++)
		{
			if( bincmp(result[i-1],result[i])!=0)
				printf("La palabra %d: \"%s\" EXISTE\n",i,result[i-1]);
		}
		printf("La palabra %d: \"%s\" EXISTE. \n",i,result[sizeResult-1]);
		*/
		for(i = 0; i < pal_con_size; i++)
		{	
			free(*(pal_con_largo+i));
		}
		free(pal_con_largo);
		for(i=0;i<sizeResult;i++)
		{
			free(*(result+i));
		}
		free(result);
		return 0;


}

int cmp(const void *s1,const void *s2)
{
  
	return  strcmp(* (char * const *) s1, * (char * const *) s2);//compara dos palabras normalmente

}

int bincmp(const void *s1,const void *s2)
{
  
	return  strncasecmp((char*) s1, (char*) s2,strlen((char*)s2))+strncasecmp((char*) s1, (char*) s2,strlen((char*)s1)); 
	//quita los espacios de las palabras del diccionario y mayúsculas, y compara correctamente

}
int bin2cmp(const void *s1,const void *s2)
{
  
	return  strcmp((char *) s1, (char*) s2); 
	//quita los espacios de las palabras del diccionario y mayúsculas, y compara correctamente

}
int busquedaBin(char ** palabras,char *buscada,int size){
   int first = 0;
   int last = size - 1;
   int middle = (first+last)/2;
 
   while (first <= last) {
      if (bin2cmp(palabras[middle],buscada)<0)
         first = middle + 1;    
      else if (!bin2cmp(palabras[middle],buscada)) {
         return middle+1;
         break;
      }
      else
         last = middle - 1;
 
      middle = (first + last)/2;
   }
   if (first > last)
      return 0;
}
int contiene(char ** palabras,char *buscada,int size){
	int i;
	for(i=0;i<size;i++)
	{
		if(!bincmp(palabras[i],buscada))
		{
			//printf("%s %s\n",palabras[i],buscada);
			return 1;
		}
	}
	return 0;
}
void swap(char *a,char * b)
{
    char temp;
    temp= *a;
    *a=*b;
    *b=temp;
}
void generarPerm(char * A,char ** perm)//
{
	int n = strlen(A);
    int c[n],i,cant,tam=factorial(n);
	strncpy(perm[0],A,n+2);
    for(i = 0; i < n; i++)
        c[i]= 0;

    //printf("%s\n",A);

	cant=1;
    i = 0;
    while( i < n )
    {
        if(c[i] < i)
		{
            if( i%2==0)		//i is even then
                swap(&A[0],&A[i]);
            else
                swap(&A[c[i]],&A[i]);
            //printf("%s\n",A);
			if(cant<tam)
				strncpy(perm[cant],A,n+2);
			cant++;
			
            c[i]++;
            i = 0;
		}
        else
		{
            c[i]= 0;
            i ++;
        }
    }
}
unsigned int factorial(unsigned int n)
 {
 	unsigned int retval = 1,i;
 	for (i = n; i > 1; --i)
 		retval *= i;
 	return retval;
 }
void strBarN(char *origen){//saca el enter final
	
	int largo = strlen(origen),i=0;
    int lim=largo;
	origen+=largo - 1;
	if(*origen=='\n')
	{
		//origen--;
		*origen = '\0';
		origen-=(largo-1);
	}
}
/*One can enumerate all k-combinations of a given set S of n elements in some fixed order, which establishes a bijection from an interval of {\displaystyle {\tbinom {n}{k}}} {\tbinom {n}{k}} integers with the set of those k-combinations.
A simple, faster way is to track k index numbers of the elements selected, starting with {0 .. k−1} (zero-based) or {1 .. k} (one-based) as the first allowed k-combination and then repeatedly moving to the next allowed k-combination by incrementing the last index number if it is lower than n-1 (zero-based) or n (one-based) or the last index number x that is less than the index number following it minus one if such an index exists and resetting the index numbers after x to {x+1, x+2, …}.*/
void generarcomb(int m, int n,char *palabra,char ** comb)//,
{
	int i,c[m],indice=0;
	char aux[n+2];
	aux[n+1]='\0';
	for (i = 0; i < n; i++) c[i] = n - i;
 
	while (1) {
		for (i = n; i--;)
		{ 
			aux[i]=palabra[c[i]-1];
			if(i==0){
				strncpy(comb[indice],aux,n);
				comb[indice][n]='\0';
				//printf("aux:%d, comb:%d\n",strlen(aux),strlen(comb[indice]));
				indice++;
			}
		}
 
		/* this check is not strictly necessary, but if m is not close to n,
		   it makes the whole thing quite a bit faster */
		i = 0;
		if (c[i]++ < m) continue;
 
		for (; c[i] >= m - i;) if (++i >= n) return;
		for (c[i]++; i; i--) c[i-1] = c[i] + 1;
	}
}

