# Convection–diffusion equation

## Methods

To find a solution, you need to use one of the following difference schemes:
1. Angle method
    <p align="center">
    <img alt="Light" src="README/angle.png" width="7%">
    <img alt="Light" src="README/angle-sch.png" width="45%">
    </p>
2. Four-points method
    <p align="center">
    <img alt="Light" src="README/4p.png" width="10%">
    <img alt="Light" src="README/4p-sch.png" width="70%">
    </p>
3. Three-points method
    <p align="center">
    <img alt="Light" src="README/3p.png" width="10%">
    <img alt="Light" src="README/3p-sch.png" width="65%">
    </p>
4. Cross method
    <p align="center">
    <img alt="Light" src="README/cross.png" width="10%">
    <img alt="Light" src="README/cross-sch.png" width="65%">
    </p>

## Results and Discussion
Given system is:

$$ 
\begin{cases}
u + c \cdot u = t \cdot x,\\
u(0,x) = \frac{x^{3}}{12 \cdot c^{2}},\\
u(t,0) = \frac{t^{3} \cdot c}{12}, \\
c = 1
\end{cases}
$$

#### Analytical solution for this system:
\* Taken from [here](https://ozlib.com/823935/informatika/uravnenie_perenosa).

<p align="center">
    <img alt="Light" src="https://ozlib.com/htm/img/7/20357/642.png" width="45%">
    <img alt="Light" src="https://ozlib.com/htm/img/7/20357/643.png" width="45%"> 
    &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
    <small>Numerical solution of the equation according to the difference scheme: a - when the Courant condition is met; b - with violation of the Courant condition.</small>
</p>

#### Numerical solution for the given system

<p align="center">
    <img alt="Light" src="2D.jpeg" width="85%">
</p>

<p align="center">
    <img alt="Dark" src="3D.jpeg" width="45%">
    &nbsp; &nbsp; &nbsp; &nbsp;
    <img alt="Dark" src="3d_rotating.gif" width="45%">
</p>
