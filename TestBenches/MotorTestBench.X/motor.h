typedef struct motor{
    int step;
    int enable;
    int count;
} Motor;
void Motor_step(Motor *m);
void Motor_enable(Motor *m);
void Motor_disable(Motor *m);
void Motor_init(Motor *m);

