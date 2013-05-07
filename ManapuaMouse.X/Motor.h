typedef struct motor{
    int step;
    int enable;
    double count;
    int speed;
    int dir; 
} Motor;
void Motor_step(Motor *m);
void Motor_enable(Motor *m);
void Motor_disable(Motor *m);
void Motor_init(Motor *m);
void Motor_reverseDir(Motor *m);
void Motor_changeDir(Motor *m);
