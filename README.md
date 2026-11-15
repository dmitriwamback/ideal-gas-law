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

| Gas | Formula | $a \ (bar \cdot L^2/mol^2)$ | $b \ (L/mol)$ |
| --- | --- | --- | --- | 
| Hydrogen | H₂ | 0.2476 | 0.02661 |
| Helium | He | 0.0346 | 0.0238 |
| Nitrogen | N₂ | 1.370 | 0.0387 |
| Oxygen | O₂ | 1.382 | 0.03186 |
| Carbon Dioxide | CO₂ | 3.640 | 0.04267 |
| Water Vapor | H₂O | 5.536 | 0.03049 |
| Methane | CH₄ | 2.283 | 0.04278 |
| Ammonia | NH₃ | 4.225 | 0.0371 |
| Sulfur Dioxide | SO₂ | 6.803 | 0.05636 |
| Chlorine | Cl₂ | 6.579 | 0.05622 |
| Neon | Ne | 0.2135 | 0.01709 |
| Argon | Ar | 1.355 | 0.03201 |
| Krypton | Kr | 2.349 | 0.03978 |
| Xenon | Xe | 4.250 | 0.05105 |
| Radon | Rn | 6.601 | 0.06239 |
| Carbon Monoxide | CO | 1.505 | 0.03985 |
| Nitrous Oxide | N₂O | 3.832 | 0.04415 |
| Hydrogen Sulfide | H₂S | 4.544 | 0.04339 |
| Phosgene | COCl₂ | 9.171 | 0.0831 |
| Ethylene | C₂H₄ | 4.612 | 0.0582 |
| Acetylene | C₂H₂ | 4.516 | 0.0522 |
| Propane | C₃H₈ | 8.779 | 0.08445 |
| Butane | C₄H₁₀ | 14.66 | 0.1226 |
| Pentane | C₅H₁₂ | 19.26 | 0.146 |
| Hexane | C₆H₁₄ | 24.71 | 0.1735 |
| Heptane | C₇H₁₆ | 31.06 | 0.2049 |
| Octane | C₈H₁₈ | 37.88 | 0.2374 |