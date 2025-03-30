#include "graph_tests.h"


runtime_test_result time_of_c_algs_runtime(double_t (*func)(graph_t*), graph_t* g)
{
    /* measures time_in miliseconds */
    clock_t begin;
    clock_t end;
    begin = clock();
    double_t sum = func(g);
    end = clock();
    double_t delta = (double_t)(end - begin) / (CLOCKS_PER_SEC /1000);
    runtime_test_result res = {sum, delta};
    return res;
}

runtime_test_result_ccp time_of_cpp_algs_runtime(double_t (*func)(weight**, int* n), graph_t* g)
{
    /* measures time_in miliseconds */
    clock_t begin;
    clock_t end;
    begin = clock();
    double_t sum = func(g->G, &g->N);
    end = clock();
    double_t delta = (double_t)(end - begin) / (CLOCKS_PER_SEC / 1000);
    runtime_test_result_ccp res = {sum, delta};
    return res;
}

void limit_of_runtime(double_t (*func)(graph_t*))
{
    int n = 1;
    graph_t* g;
    
    double_t delta = 0.0;
    do 
    {
        g = get_random_euclidean_graph(n);   
        
        delta = time_of_c_algs_runtime(func, g).delta;
        n += 1000;

        free(g->G);
        free(g);
        printf("%d,%f\n", n-1, delta);
    } while ( (delta < 180000.0));

    return;
}

void tests(int v_min, int v_max, int v_max_bf, int n_tests, FILE* fp)
{
    graph_t* g;

    // resuling arrays
    double_t* bf_avg_time = (double_t*)malloc((v_max - v_min + 1) * sizeof(double_t));
    double_t* eager_avg_time = (double_t*)malloc((v_max - v_min + 1) * sizeof(double_t));
    double_t* dtree_avg_time = (double_t*)malloc((v_max - v_min + 1) * sizeof(double_t));

    double_t* eager_avg_distance = (double_t*)malloc((v_max - v_min + 1) * sizeof(double_t));
    double_t* dtree_avg_distance = (double_t*)malloc((v_max - v_min + 1) * sizeof(double_t));

    
    double_t bf_total_time = 0;
    double_t eager_total_time = 0;
    double_t dtree_total_time = 0;

    double_t bf_min_dist = 0;
    double_t eager_min_dist = 0;
    double_t dtree_min_dist = 0;

    double_t eager_total_distance = 0;
    double_t dtree_total_distance = 0;

    int eager_number_of_best = 0;
    int dtree_number_of_best = 0;

    runtime_test_result bf_res;
    runtime_test_result eager_res;
    runtime_test_result_ccp dtree_res;

    fprintf(fp, "n,number_of_tests,bf_avg_time,eager_avg_time,eager_avg_dist,dtree_avg_time,dtree_avg_dist,eager_match_best,dtree_match_best\n");

    for (int n = v_min; n <= v_max; n++)
    {
        bf_total_time = 0;
        eager_total_time = 0;
        dtree_total_time = 0;
        bf_min_dist = 0;

        eager_min_dist = 0;
        dtree_min_dist = 0;

        dtree_total_distance = 0;
        eager_total_distance = 0;

        eager_number_of_best = 0;
        dtree_number_of_best = 0;

        // cycle over number of tests
        for (int i = 0; i < n_tests; i++)
        {            
            g = get_random_euclidean_graph(n);
            
            eager_res = time_of_c_algs_runtime(&eager_tsp, g);
            dtree_res = time_of_cpp_algs_runtime(&getCycleDoubleTree, g);

            eager_min_dist = eager_res.sum;
            dtree_min_dist = dtree_res.sum;
            

            if (n < v_max_bf)
            {
                bf_res = time_of_c_algs_runtime(&naive_brute_force, g);
                bf_min_dist = bf_res.sum;
                
                if(abs(bf_min_dist - eager_min_dist) < EPS)
                    eager_number_of_best++;

                if(abs(bf_min_dist - dtree_min_dist) < EPS)
                    dtree_number_of_best++;
            
                dtree_total_distance += (dtree_min_dist-bf_min_dist) / bf_min_dist;
                eager_total_distance += (eager_min_dist-bf_min_dist) / bf_min_dist;
                    
                bf_total_time += bf_res.delta;
            }
            
            eager_total_time += eager_res.delta;
            dtree_total_time += dtree_res.delta;


            for(int k = 0; k < g->N; k++)
                free(g->G[k]);
            free(g->G);
            free(g);
        }   

        bf_avg_time[n-v_min] = bf_total_time / (n_tests);
        eager_avg_time[n-v_min] = eager_total_time / (n_tests);
        dtree_avg_time[n-v_min] = dtree_total_time / (n_tests);

        eager_avg_distance[n-v_min] = eager_total_distance / (n_tests); 
        dtree_avg_distance[n-v_min] = dtree_total_distance / (n_tests); 
        
        fprintf(fp, "%d,%d,", n, n_tests);
        fprintf(fp, "%f,", bf_avg_time[n-v_min]);

        fprintf(fp, "%f,", eager_avg_time[n-v_min]);
        fprintf(fp, "%f,", eager_avg_distance[n-v_min]);

        fprintf(fp, "%f,", dtree_avg_time[n-v_min]);
        fprintf(fp, "%f,", dtree_avg_distance[n-v_min]);
 
        fprintf(fp, "%d,%d\n", eager_number_of_best, dtree_number_of_best);
    }

    free(bf_avg_time);
    free(eager_avg_time); 
    free(dtree_avg_time); 
    free(eager_avg_distance);
    free(dtree_avg_distance);
}