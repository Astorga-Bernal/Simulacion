/* External definitions for single-server queueing system. */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "lcgrand.h"  /* Header file for random-number generator. */

#define Q_LIMIT 100  /* Limit on queue length. */
#define BUSY      1  /* Mnemonics for server's being busy */
#define IDLE      0  /* and idle. */

void  initialize(void);
void  timing(void);
void  arrive(void);
void  depart(void);
void  report(void);
void  update_time_avg_stats(void);
float expon(float mean);
float gen_next_interarrive(void);
float gen_next_service_time(void);
float  gen_next_depart(void);
int next(float x, float y);

float sim_time;
float num_custs_delayed;

float num_delays_required;
float total_of_delays;
float area_num_in_q;
float area_server_status;

float queue[100];
float time_next_arrive;
float time_next_depart;

int next_event_type;
int server_status;
int num_in_q;
float time_last_event;

main()  /* Main function. */
{

    /* Initialize the simulation. */
    initialize();

    /* Run the simulation while more delays are still needed. */

    while (num_custs_delayed < num_delays_required) {

        /* Determine the next event. */
        timing();
        /* Update time-average statistical accumulators. */

        update_time_avg_stats();

        /* Invoke the appropriate event function. */

        switch (next_event_type) {
            case 1:
                printf("Arribo en tiempo: %2f \n", sim_time);
                arrive();
                break;
            case 2:
                printf("Salida en tiempo: %2f \n", sim_time);
                depart();
                break;
        }
        printf("Numero de clientes atendidos: %2f \n", num_custs_delayed);
    }

    /* Invoke the report generator and end the simulation. */

    /* report();*/

    return 0;
}


void initialize(void)  /* Initialization function. */ /*--inicializacion de las variables de la simulacion--*/
{
    /* Initialize the simulation clock. */

    sim_time = 0.0; /*--reloj de la simulacion--*/
    num_delays_required = 100;

    /* Initialize the state variables. */

    server_status   = IDLE; /*--estado del/los servidor/es BUSY|IDLE--*/
    num_in_q        = 0; /*--nuemero de clientes en cola--*/
    time_last_event = 0.0; /*--tiempo del ultimo evento--*/

    /* Initialize the statistical counters. num_custs_delayed, total_of_delays, area_num_in_q, area_server_status */
    num_custs_delayed = 0;
    total_of_delays = 0.0;
    area_num_in_q = 0.0;
    area_server_status = 0.0;

    /* Initialize event list.  Since no customers are present, the departure */
    /* (service completion) event is eliminated from consideration. */
    /*arreglos donde se guarda en el primer elemento el tiempo de la proximo arribo y en el segundo */
    /*elemento el tiempo de la proxima salida --se podria hacer con 2 variables --*/
    time_next_arrive = sim_time + gen_next_interarrive() ; /* Planificacion del primer arribo */
    time_next_depart = FLT_MAX; /* infinito --si tengo multiples servidores deberia tener una lista del mismo # de servidores para la salida */
}


void timing(void)  /* Timing function. */ /*--se fija el minimo entre el proximo tiempo de entrada y el proximo tiempo de salida--*/
{
    int   i;
    float min_time_next_event = 1.0e+29;

    next_event_type = 0; /* Al final debe quedar en 1 en caso de que el proximo evento sea un arribo, 2: si es la salida de un servicio */

    /* Determine the event type of the next event to occur. */
/*    printf("La proxima entrada es %2f \n",time_next_event[1]);
    printf("La proxima salida es %2f \n",time_next_event[2]);*/

    next_event_type = next(time_next_arrive,time_next_depart);

    /* advance the simulation clock. */
    if(next_event_type == 1){
        sim_time = time_next_arrive;
    }else{
        sim_time = time_next_depart;
    }
}


void arrive(void)  /* Arrival event function. */
{
    float delay;

    /* Schedule next arrival. */

    time_next_arrive = sim_time + gen_next_interarrive();

    /* Check to see whether server is busy. */

    if (server_status == BUSY) {

        /* Server is busy, so increment number of customers in queue. */

	    num_in_q = num_in_q + 1.0;
        /* Check to see whether an overflow condition exists. */

        if (num_in_q > Q_LIMIT) {
            exit(2); /* cuando no tengo mas lugar para encolar clientes termina la simulacion */
        }else{
            queue[num_in_q] = gen_next_service_time(); 
        }

        /* Guardar el tiempo de arribo de esta entidad para los calculos estadisticos */ 
	    /*es para calcular el tiempo que espero en cola*/

    }

    else {

        /* Server libre, tener en  cuenta la entidad que pasa directamente al server para los calculos estadisticos */
        server_status = BUSY;

        /* Schedule a departure (service completion). */

        time_next_depart = sim_time + gen_next_service_time(); /* generar con una funcion (con una distribucion estadistica) el tiempo de servicio */
    }
}


void depart(void)  /* Departure event function. */
{
    int   i;
    float delay;

    /* Check to see whether the queue is empty. */

    if (num_in_q == 0) {

        /* The queue is empty so make the server idle and eliminate the */
        /* departure (service completion) event from consideration. */
        server_status = IDLE; 
    }
    else {

        /* The queue is nonempty, so decrement the number of customers in
           queue. */ 

        /* Compute the delay of the customer who is beginning service and update
           the total delay accumulator. */


        /* Increment the number of customers delayed, and schedule departure. */


        time_next_depart = sim_time + queue[num_in_q];
        num_in_q = num_in_q -1;
        num_custs_delayed = num_custs_delayed + 1;

    }
}


void report(void)  /* Report generator function. */
{
    /* Compute and write estimates of desired measures of performance. */

    /*\\Average delay in queue*/

    /*\\Average number in queue*/

    /*\\Server utilization*/
}


void update_time_avg_stats(void)  /* Update area accumulators for time-average statistics. */
{
    float time_since_last_event;

    /* Compute time since last event, and update last-event-time marker. */

    time_since_last_event = sim_time - time_last_event;
    time_last_event       = sim_time;

    /* Update area under number-in-queue function. */



    /* Update area under server-busy indicator function. */

}


float gen_next_interarrive()  
{
  return 2.0;
}

float  gen_next_depart()
{
  return 2.0;
}

float gen_next_service_time()
{
  return 2.0;
}

int next(float x, float y)
{
    if(x<=y)
    {
       return 1;
    }else{
       return 2;
    }
}