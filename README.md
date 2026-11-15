# Softbody Simulation using the Van der Waals Equation


## How to run

## Theory

### Van der Waals equation

<p>Van der Waals equation for gases:</p>

$$ (P + \frac{an^2}{V^2})(V - nb) = nRT $$

<p>Then isolate the pressure:</p>

$$ P = \frac{nRT}{V - nb} - \frac{an^2}{V^2} $$

<p>Where:</p>

1. $P$ is the pressure ($bar = 10^5Pa$)

2. $V$ is the volume ($L$)

3. $n$ is the number of moles ($mol$)

4. $R$ is the gas constant ($\frac{0.0831446 L⋅bar}{K⋅mol}$)

5. $T$ is the temperature ($K$)

6. $a$ and $b$ are substance-specific constants. $a$ describes the attractive forces whereas $b$ describes the physical volume occupied by the gas.

<p></p>

Then, with this pressure we find $ΔP = P - P_{ext}$ where $P_{ext}$ is the atmospheric pressure ($1 \ atm$ or $1.01325 \ bar$).

<p></p>

To calculate the force exerted by the pressure, the normals of the 2D shape must be calculated. Assume $\vec{p}_i$ and $\vec{p}_{i+1}$ are adjacent vertices going in the counter clockwise direction. The edge $\vec{e}$ is defined $\vec{e} = \vec{p}_{i+1} - \vec{p}_i$ and thus the normal is $\vec{n} = (\vec{e}_y, \ -\vec{e}_x)$.

<p></p>

As the simulation runs, this force is calculated between two adjacent edges:

$$ \vec{F}_i = \vec{F}_{i-1} + \frac{ΔP}{2}\vec{n}_i $$

$$ \vec{F}_j = \vec{F}_{j-1} + \frac{ΔP}{2}\vec{n}_j $$

<p></p>

Such that $i, j$ correspond to two individual adjacent normals.

<p></p>

For each neighboring pair, calculate $\vec{d} = \vec{p}_{i+1} - \vec{p}_i$, $L = |\vec{d}|$, and $\hat{d} = \vec{d}/L$. With these, we can calculate the spring force with damping:

$$ \vec{F}_s = [k_s(L - L_0) + k_d(\vec{v}_j - \vec{v}_i) \cdot \hat{d}]\hat{d} $$

<p></p>

And we can compute a force that pulls each point toward the midpoint of its neighbors:

$$ \vec{F}_m = k_b(\frac{\vec{p}_{i-1} + \vec{p}_{i+1}}{2} -\vec{p_i} ) $$

<p></p>

Where:

1. $k_s$ is the spring constant.
2. $k_d$ is the damping spring constant.
3. $L_0$ is the length of the edge between $\vec{p}_i$ and $\vec{p}_{i+1}$ (fixed).

<p></p>

This force is applied to both $\vec{p}_i$ and in the opposite for $\vec{p}_j$ such that:

$$ \vec{F}_{i+1} = \vec{F}_{i} + \vec{F}_s + \vec{F_g} + \vec{F}_m $$
$$ \vec{F}_{j+1} = \vec{F}_{j} - \vec{F}_s + \vec{F_g} + \vec{F}_m $$

<p></p>

Both $\vec{F}_{i+1}$ and $\vec{F}_{j+1}$ represent $\vec{F}_{net}$ ($m\vec{a}$) for both points $i,j$ in the current frame.

<p></p>

With our $\vec{F}_{net}$ for both objects, we can modify the positions of the vertices:

<p></p>

$$ \vec{v}_i = \vec{v}_{i-1} + \frac{\vec{F}}{m}Δt $$

$$ \vec{p}_i = \vec{p}_{i-1} + \vec{v}_iΔt$$

<p></p>

### Table of $a$, $b$ values for the Van der Waals equation

<p></p>

Table of gases and their $a$ and $b$ values.

| Substance | Formula | $a \ (bar \cdot L^2/mol^2)$ | $b \ (L/mol)$ |
| --- | --- | --- | --- |
| Helium | $He$ | 0.0346 | 0.02380 |
| Hydrogen | $H_2$ | 0.2453 | 0.02650 |
| Nitrogen | $N_2$ | 1.3700 | 0.03870 |
| Oxygen | $O_2$ | 1.3820 | 0.03186 |
| Carbon Dioxide | $CO_2$ | 3.6580 | 0.04286 |
| Ammonia | $NH_3$ | 4.2250 | 0.03713 |
| Water Vapor | $H_2O$ | 5.5370 | 0.03049 |
| Methane | $CH_4$ | 2.3000 | 0.04301 |