# Softbody Simulation using the Van der Waals Equation

<p>Van der Waals equation for gases:</p>

$$ (P + \frac{an^2}{V^2})(V - nb) = nRT $$

<p>Where:</p>

1. $P$ is the pressure (bar)

2. $V$ is the volume (L)

3. $n$ is the number of moles (mol)

4. $R$ is the gas constant

5. $T$ is the temperature (C)

6. $a$ and $b$ are substance-specific constants. $a$ describes the attractive forces whereas $b$ describes the physical volume occupied by the gas.

<p></p>

In this repo, we treat $nRT$ as a constant.

<p></p>

Table of gases and their $a$ and $b$ values.

| Substance | Formula | $a (bar L^2/mol^2)$ | $b (L/mol)$ |
| --- | --- | --- | --- |
| Helium | $He$ | 0.0346 | 0.02380 |
| Hydrogen | $H_2$ | 0.2453 | 0.02650 |
| Nitrogen | $N_2$ | 1.3700 | 0.03870 |
| Oxygen | $O_2$ | 1.3820 | 0.03186 |
| Carbon Dioxide | $CO_2$ | 3.6580 | 0.04286 |
| Ammonia | $NH_3$ | 4.2250 | 0.03713 |
| Water Vapor | $H_2O$ | 5.5370 | 0.03049 |
| Methane | $CH_4$ | 2.3000 | 0.04301 |