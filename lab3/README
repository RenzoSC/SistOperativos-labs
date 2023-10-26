# LABORATORIO N°3, GRUPO 2

## Informe de Análisis: 

## PARTE 1: PREGUNTAS

### 1- Qué política de planificación utiliza xv6-riscv para elegir el próximo proceso a ejecutarse? 
    xv6-riscv utiliza la política de RR, dado un tiempo (quantum) permite que
    los procesos corran consecutivamente.
    Recorre una tabla de procesos buscando los que esten en estado RUNNABLE (Ready) para cambiarlos al estado de RUNNING y hacer
    el cambio de contexto.
    Se puede ver en la implementación de scheduler() en proc.c


### 2- ¿Cuánto dura un quantum en xv6-riscv?
    Un quantum dura cerca de 1/10 second en qemu. Esto lo podemos ver en el archivo
    start.c en la función timerinit() que es el que se encarga de recibir interrupts

    // ask the CLINT for a timer interrupt.
    int interval = 1000000; // cycles; about 1/10th second in qemu.


### 3- ¿Cuánto dura un cambio de contexto en xv6-riscv?
        Observando el archivo swtch.s vemos el siguiente código en assembler:

    swtch:
        sd ra, 0(a0)
        sd sp, 8(a0)
        sd s0, 16(a0)
        sd s1, 24(a0)
        sd s2, 32(a0)
        sd s3, 40(a0)
        sd s4, 48(a0)
        sd s5, 56(a0)
        sd s6, 64(a0)
        sd s7, 72(a0)
        sd s8, 80(a0)
        sd s9, 88(a0)
        sd s10, 96(a0)
        sd s11, 104(a0)

        ld ra, 0(a1)
        ld sp, 8(a1)
        ld s0, 16(a1)
        ld s1, 24(a1)
        ld s2, 32(a1)
        ld s3, 40(a1)
        ld s4, 48(a1)
        ld s5, 56(a1)
        ld s6, 64(a1)
        ld s7, 72(a1)
        ld s8, 80(a1)
        ld s9, 88(a1)
        ld s10, 96(a1)
        ld s11, 104(a1)
        
        ret

        Si queremos tener de cierta forma alguna medición aproximada de cuanto tarda el cambio de contexto deberíamos
        calcular cuanto tiempo tarda la cantidad de operaciones que realiza internamente.
        Las operaciones sd y ld duran dos ciclos de reloj, por lo que teniendo 14 de cada una, tenemos:
        28 * 2 = 56 ciclos de reloj es lo que dura un cambio de contexto. 




### 4- ¿El cambio de contexto consume tiempo de un quantum?
    El cambio de contexto se realiza en swtch.S, bajo la función del mismo nombre, donde se guardan los registros del old context 
    (El del proceso que dejo la CPU) y se cargan los del new context (nuevo proceso que se carga en la CPU). 
    La cantidad de instrucciones ejecutadas por el procesador (guardado y cargado del estado de los procesos) son 28, lo cual implica 
    un consumo, aunque ínfimo, de tiempo en la realización del context switch.
    En conclusión, el cambio de contexto sí consume tiempo de un quantum ya que este es asignado como interrupción de forma global al 
    SO y no a un proceso en particular. 



### 5- ¿Hay alguna forma de que a un proceso se le asigne menos tiempo? 
    El quantum es asignado mediante un timer interrupt al Sistema Operativo de forma global, no de forma particular a cada proceso.
    Motivo de esto, puede suceder que un proceso termine antes de la finalización del quantum, por lo que el siguiente elegido por el 
    scheduler
    va a tener un tiempo menor asignado (el faltante) ya que comienza con el tiempo ya transcurrido.


### 6- ¿Cúales son los estados en los que un proceso pueden permanecer en xv6-riscv y que los hace cambiar de estado?
    Los estados en los que puede permanecer un proceso en xv6-rscv son:
    
    { UNUSED, USED, SLEEPING, RUNNABLE, RUNNING, ZOMBIE }
    
    Los podemos ver explicitamente en el archivo proc.h
    Lo que los hace cambiar de estado son las distintas llamadas a funciones que se
    dan a lo largo de la vida de un proceso. Por ejemplo si llamamos a exit(), el proceso
    pasa a tener un proc->state = ZOMBIE, o bien en la función yield() que el proceso pasa a tener un estado RUNNABLE. 


## ¿Como implementamos la MLFQ?
La MLFQ la implementamos utilizando un array que contiene los procesos. Antes de entrar al for que elige qué proceso correr
copiamos los procesos que se encuentran en la PROC TABLE y los ordenamos en base a su prioridad. La función que decide el orden
de los procesos es el goes_before()

```
/*
This functions helps us to find the order between two procs
1->  a goes before b
0->  b goes before a
The order is in first instance determined by prio

prio A > prio B

if prio A == prio B
  the order is determined by select counter
  select counter A < select counter B
*/
int goes_before(struct proc *a, struct proc *b){
  if (a->prio > b->prio)
  {
    return 1;
  }else if (b->prio > a->prio)
  {
    return 0;
  }else if (a->prio == b->prio && (a->select_counter > b->select_counter))
  {
    return 0;
  }else if (a->prio == b->prio && (a->select_counter < b->select_counter))
  {
    return 1;
  }else
  {
    return 1;
  }
}
```

El algoritmo que utilizamos para la ordenación sería el insertion sort.

```
for(;;){
    // Avoid deadlock by ensuring that devices can interrupt.
    intr_on();
    
    int i=0;
    for (p = proc; p < &proc[NPROC]; p++)
    {
      prio[i] = p;
      i++;
    }
    insertion_sort(prio, NPROC);
    for(i = 0; i < NPROC; i++) {
      struct proc *p = prio[i];
      acquire(&p->lock);
      if(p->state == RUNNABLE) {
        // Switch to chosen process.  It is the process's job
        // to release its lock and then reacquire it
        // before jumping back to us.
        p->state = RUNNING;
```

En esta sección de código podemos ver como siempre antes de elegir el proceso a correr ordenamos el array con los procesos,
dejando así en los primeros lugares los procesos que deberían tener más prioridad. Claramente solo ejecutará aquellos que tengan un
estado RUNNABLE

```
p->select_counter +=1;
        
        c->proc = p;
        swtch(&c->context, &p->context);
        
        // Process is done running for now.
        // It should have changed its p->state before coming back.
        c->proc = 0;
        
        acquire(&tickslock);
        p->last_exec = ticks;           //Esto nos dice la ultima vez q fue ejecutado una vez terminó de correr el proceso
        release(&tickslock);
        release(&p->lock);
        break;
      }
      release(&p->lock);
    }
  }
```

Esta otra parte del scheduler podemos ver que una vez el proceso se ejecutó y terminó... realizamos un break para que se **deje de**
**iterar sobre el array ordenado** dejando así la posibilidad de que se vuelva a ordenar ya que la prioridad o la cantidad de selección
puede variar después de la ejecución de un proceso. Si no hubieramos agregado este break el comportamiento de nuestra MLFQ hubiera sido
muy similar a un RR.


### ¿Se puede producir starvation en el nuevo planifi-cador? Justifique su respuesta.
Sí, se puede producir. Imaginemos que tenemos un proceso que justo antes de que acabe el quantum produce un llamado de entrada/salida.
Este proceso por las reglas de la MLFQ que nos dieron, siempre mantendrá su prioridad (la más alta) y el scheduler implementado con
esta versión de MLFQ lo elegirá siempre, impidiendo así que otros procesos se ejecuten.



## ANÁLISIS DE LOS DISTINTOS CASOS DE EXPERIMENTO: 

### A continuación vamos a dar un análisis de los resultados obtenidos de las medidas requeridas de los distintos casos: 

Observando primero los resultados de las mediciones obtenidas del **caso 1 y 2**, con la política de RR, nos podemos dar cuenta 
que al correr solo un proceso **IOBENCH** y luego solo un proceso **CPUBENCH**, la cantidad de veces que se selecciona a un proceso 
**IOBENCH** es mucho mayor a la cantidad de veces que se selecciona un proceso CPU. Esto se debe a que **IOBENCH** realiza muchas 
llamadas de entrada/salida dentro de un quantum, por lo tanto al ejecutar **IOBENCH** ocupa muy poco tiempo en el CPU, y al ser el 
único proceso en la lista de procesos se elegirá muchas veces hasta que el proceso termine. 
Caso contrario sucede en el **CPUBENCH**, ya que el proceso al tener mucho cálculo ocupa todo el tiempo del quantum y por lo tanto se 
selecciona muchas menos veces que el **IOBENCH**. Esto también sucede en el caso de la política MLFQ ya que solamente se encuentra un 
proceso en la lista de procesos. 

Cuando tenemos un quatum 10 veces más chico ocurre lo mismo para ambas políticas ya que estamos corriendo un solo proceso a la vez, 
con la diferencia de que el proceso **CPUBENCH** aumenta la cantidad de veces seleccionado, pues al tener un quantum más chico aumenta 
la cantidad de veces en el que el proceso es interrumpido por el quantum.  

### La siguiente observación son los resultados del caso 3, con la política de RR:

Podemos ver que para ambos procesos, tanto como para el **CPUBENCH** como para el **IOBENCH**, se reduce la cantidad promedio de
operaciones cada 100 ticks. Esto se debe a que ambos procesos corren intercaladamente compartiendo su tiempo en la CPU, antes teníamos
que se corría un solo proceso (ya sea IO/CPU BENCH) y no tenía necesidad de compartir recursos, ahora como son dos procesos sí los
tiene que compartir. El proceso **CPUBENCH** consume todo un quantum y pasado esto el scheduler decide dejar de ejecutarlo para pasar
a ejecutar el proceso **IOBENCH**, este realiza su llamada de entrada/salida y finaliza antes de que termine el quantum, dando paso a
que el ciclo vuelva a empezar (ya que solo son estos dos procesos los únicos que estan en la PROCLIST) y empieza nuevamente por el
**CPUBENCH**. 

Análogo a lo anterior, con el quantum más chico, vemos que se favorece el **IOBENCH** ya que aumenta significativamente la cantidad 
de operaciones promedio acercándose así al **CPUBENCH**. Esto sucede debido a que, con el quantum 10 veces más chico, la cantidad de
intercalaciones que se produce entre la selección de procesos es mayor, dándole así 10 veces más selecciones a los procesos.
Ahora si bien ambos procesos son seleccionados 10 veces más por igual ¿por qué supone un aumento solo para el **IOBENCH**?
Esto se debe a que cuando un proceso **IOBENCH** es elegido, hace un llamado de entrada/salida por selección, dandole así luego paso
a que se ejecute un siguiente proceso. Entonces si previamente en 1s hacía 10 llamados de entrada/salida, con un quantum 10 veces
menor, ahora en 1s produce 100 llamados de entrada/salida, ya que las veces que fue seleccionado son 10 veces más grandes.
En el proceso **CPUBENCH** esto no pasa ya que el promedio de MFLOPS no depende de cuantas veces fue seleccionado, sino de cuanto
tiempo fue ejecutado, y al achicar el quantum 10 veces más no estamos cambiando el tiempo que se ejecuta. El tiempo para el 
**CPUBENCH** sigue siendo el mismo ya que ocupa todo un quantum (independientemente de cuanto sea el quantum).


Por otro lado, siguiendo con el análisis del caso 3, pero con la política de **MLFQ**, vemos que sucede lo contrario a la política de 
**RR** ya que esta política favorece a los procesos de entrada/salida y desfavorece a los procesos que ocupan mucho tiempo en el 
procesador. El proceso **IOBENCH** realiza muchas más operaciones que el CPUBENCH, esto es porque los procesos que dejan el CPU antes 
de que termine el quantum tienen más prioridad que los procesos que consumen el quantum completo. Y como nuestra **MLFQ** está hecha
para que ejecute procesos que tienen mayor prioridad, en general va a ejecutar MUCHO más veces el proceso **IOBENCH** por ser que
tiene más prioridad. 


Al tener un quantum más chico sucede algo peculiar. (Lo cual le preguntamos a un profesor del lab buscando una explicación)
En un principio el proceso **IOBENCH** es el que equipara todo el tiempo de cpu ya que tiene más prioridad. Y como el quantum es 10
veces más chico, es elegido más veces y genera más operaciones entrada/salida sin darle oportunidad al **CPUBENCH** se ejecute casi. 
**Pero** a su vez, como el quantum es más chico, también genera la posibilidad de que el **IOBENCH** termine JUSTO al mismo tiempo que 
el quantum, produciendo así que baje su prioridad y al llegar a tener misma prioridad que el proceso **CPUBENCH**, al ser este el que 
menos veces fue elegido (ya que previamente el IOBENCH fue el que equiparó todo el tiempo de CPU) empieza a ser ejecutado solo el 
**CPUBENCH** intentando llegar a la misma cantidad de selecciones que el **IOBENCH**. Una vez lo alcanza le da la oportunidad de 
ejecutar al IO ya que estarían en la misma prioridad, y así el IO vuelve a su naturaleza anterior (generando más operaciones entrada/
salida y sin dejar que el CPUBENCH se ejecute) hasta que vuelva a pasar esta anomalía. 

La explicación que nos dió el profe de por qué podría suceder esto es debido a que justo el IOBENCH termina cuando el quantum termina,
entonces se produce que su prioridad se reduce. Con esto sacamos la conclusión de lo explicado anteriormente 

### Análisis del caso 4:

En el caso de RR la cantidad de selecciones de cada proceso **CPUBENCH** se ve dividida en 2 (Cuando teníamos un solo proceso
CPUBENCH corriendo en general terminaba con 2100 selecciones, pero en este caso cada uno terminaba con 1000-1100 selecciones). La
cantidad promedio de MFLOPS no se ve afectada.

En el caso de MLFQ al ser dos procesos **CPUBENCH** que mantienen su prioridad en 0, la MLFQ se comporta como un RR. Nos dimos cuenta
que la MLFQ que nos hicieron implementar se comporta similar a  RR sobre procesos que tienen la misma prioridad (y no difieren en más 
de 1 en el selec_counter). Esto se debe a que el MLFQ entre los procesos que tienen la misma prioridad elije al que menos fue elegido.
Imaginemos que tenemos dos procesos **CPUBENCH** A y B, A fue elegido 13 veces, y B fue elegido 14 veces. Nuestra MLFQ elige primero
al proceso A, ahora paso a ser elegido 14 veces, según nuestra función goes_before, ahora A debe ser elegido una vez más y pasa a
haber sido elegido 15 veces, ahora en vez de elegir A elige B y así sucesivamente. Vemos que su comportamiento es similar al de RR,
nuestra MLFQ produce **AABBAABBAABB** mientras que RR produciría **ABABABABABAB**

Con un quantum más chico el resultado es muy similar para ambos casos.

### Análisis del caso 5:

Para un planificador RR el caso 5 es muy similar al caso 3, solo que en este caso como tenemos 2 **CPUBENCH** el proceso **IOBENCH**
tarda un quantum más en ser elegido, por ende el numero de llamadas a entrada/salida se ve reducido (un poco pero notable).

Con un quantum más chico se ve el mismo efecto que el caso 3, aumenta su número de operaciones de entrada/salida, esto gracias a que
fue más veces elegido (ya que el quantum es menor).

Para el planificador MLFQ el caso 5 también es similar al caso 3 de MLFQ con quantum normal, el proceso **IOBENCH** equipara la mayoría
del tiempo de CPU impidiendo así que los procesos **CPUBENCH** queden casi sin ser atendidos.

Cuando achicamos el quantum podemos notar una combinación del efecto raro que sucede en el caso 3 con el MLFQ y el caso 4. El 
**IOBENCH** en un principio toma el control de la CPU, pero como tiene quantum más chico aumenta sus posibilidades de que termine al 
mismo tiempo q termina el quantum y así pierda su prioridad, una vez pierde su prioridad da paso a que los procesos **CPUBENCH** se
ejecuten y estos dos lo harán con el comportamiento similar a RR que explicamos en el caso 4.


## Gráficos de iobench &; cpubench en RR y MLFQ (nos pareció los más interesantes) [Haga click aquí para ver los gráficos! :)](grafico)

## Mediciones [Haga click aquí para ver las mediciones! :)](mediciones)
