import numpy as np

def forward_euler(f, t_s, x, ts_s, *args):
    for i in range(1, len(t_s)):
        x[:,i] = x[:,i-1] + ts_s * f(t_s[i-1], x[:,i-1], *args)
    
    return t_s, x