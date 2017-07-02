function [ X ] = recSTFT( x,t,f,B )
    dt = t(2)-t(1);
    df = f(2)-f(1);
    n0 = t(1)/dt;
    m0 = f(1)/df;
    T = length(t);
    F = length(f);
    Q = B/dt;
    N = 1/(dt*df);
    n=n0;
    while(n<=n0+T-1)
        for q = 0:N-1
            if(n<=q && q<=n + 2*Q && floor(n-Q+q)>=0 && floor(n-Q+q) < N)
                x1(q+1) = x(floor(n-Q+q+1)); 
            else
                x1(q+1)=0;
            end
        end
        X1 = fft(x1);
        for i = 0:F-1
            X(n-n0+1,i+1) = dt*exp(1j*2*pi/N*(Q-n)*(m0+i))*X1(i+1);
        end
        n=n+1;
    end
end

