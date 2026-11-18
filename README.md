# Softbody Simulation using the Van der Waals Equation


## How to run

```shell
cmake CMakeLists.txt
cmake --build .
./ideal_gas_law
```

### Changing gas composition (found in src/scene/Renderer.cpp)

```c++
double gasMassKg = 3000.0;
std::vector<Component> components = {
    {GasType::HeliumGas, 0.025},
    {GasType::CarbonDioxide, 0.675},
    {GasType::NitrousOxide, 0.1},
    {GasType::KryptonGas, 0.2}
};
Mixture m = makeMixture(components, gasMassKg);

SoftbodyParams params;

SoftBody softBody = SoftBody::MakeEquilibriumIcosphere(2, glm::dvec3(-2, 15, -5), m, params, 298.0);
```
In the above case, the radius/sphere size changes depending on composition.

The first element describes which gas is used and the second determines how much in kg the gas contributes.

In any case, ensure that for $k$ species,

$$ \sum_k C(k) = 1, \qquad C(k) = composition \ of \ k $$

Or the program will throw an error

## Theory

### Van der Waals equation

<p>Van der Waals equation for gases:</p>

$$ \left(P + \frac{an^2}{V^2}\right)(V - nb) = nRT $$

<p>Then isolate the pressure:</p>

$$ P = \frac{nRT}{V - nb} - \frac{an^2}{V^2} $$

<p>Where:</p>

1. $P$ is the pressure ($bar = 10^5 Pa$)

2. $V$ is the volume ($L$), here the volume enclosed by the 3D mesh

3. $n$ is the number of moles ($mol$).

4. $R$ is the gas constant ($\frac{0.0831446 L⋅bar}{K⋅mol}$)

5. $T$ is the temperature ($K$)

6. $a$ and $b$ are substance-specific constants. $a$ describes the attractive forces whereas $b$ describes the physical volume occupied by the gas.

<p></p>

<p>The simulation computes everything in SI units so that pressure × area comes out directly in newtons. The table values convert as:</p>

$$ a \ (Pa \cdot m^6/mol^2) = 0.1 \cdot a \ (bar \cdot L^2/mol^2) $$

$$ b \ (m^3/mol) = 10^{-3} \cdot b \ (L/mol) $$

$$ R = 8.314 \ J/(mol \cdot K) $$

<p></p>

### Gas mixtures

For a mixture of species $k$ with $n_k$ moles each, the one-fluid mixing rules are:

<p></p>

$$ n = \sum_k n_k \qquad b_{tot} = \sum_k n_k b_k \qquad a_{tot} = \left(\sum_k n_k \sqrt{a_k}\right)^2 $$

<p>and the pressure becomes:</p>

$$ P = \frac{nRT}{V - b_{tot}} - \frac{a_{tot}}{V^2} $$

<p></p>

For a single gas this is the same as the equation above ($b_{tot} = nb$, $a_{tot} = an^2$).

<p></p>

### Net pressure

<p>The skin only feels the difference between the inside pressure and the pressure of the surrounding air:</p>

$$ ΔP = P - P_{ext} $$

where $P_{ext}$ is atmospheric pressure ($1 \ atm = 101325 \ Pa$) and is held constant. If $ΔP > 0$ the skin is pushed outward, and if $ΔP < 0$ it is pushed inward. $V - b_{tot}$ is clamped to a small positive minimum so $P$ never divides by zero.

<p></p>

### The mesh

The balloon is a closed triangle mesh (an icosphere). Each vertex $i$ has a position $\vec{p}_i$, a velocity $\vec{v}_i$, a mass $m_i$, and an accumulated force $\vec{F}_i$. Every triangle is stored with its vertices in counter clockwise order when viewed from outside, so the normals point outward.

<p></p>

### Volume

The enclosed volume is the sum of signed tetrahedra formed between the origin and every triangle (divergence theorem). For a triangle with vertices $\vec{p}_0, \vec{p}_1, \vec{p}_2$:

$$ V = \frac{1}{6} \sum_{triangles} \vec{p}_0 \cdot (\vec{p}_1 \times \vec{p}_2) $$

If $V$ is negative, the triangles are wound inside out.

<p></p>

### Pressure force

<p>For every triangle, the (unnormalized) normal is:</p>

$$ \vec{N} = \frac{1}{2}(\vec{p}_1 - \vec{p}_0) \times (\vec{p}_2 - \vec{p}_0) $$

<p></p>

Its direction is the outward normal and its length is the area of the triangle, so $ΔP \ \vec{N}$ is already pressure × area. It is not normalized. The force is split equally between the three vertices of the triangle:

<p></p>

$$ \vec{F}_{0} \mathrel{+}= \frac{ΔP}{3}\vec{N} \qquad \vec{F}_{1} \mathrel{+}= \frac{ΔP}{3}\vec{N} \qquad \vec{F}_{2} \mathrel{+}= \frac{ΔP}{3}\vec{N} $$

<p></p>

Here $\mathrel{+}=$ means accumulate. Each vertex is shared by several triangles, so it collects a contribution from each one. All forces are set to $\vec{0}$ at the start of every substep.

<p></p>

### Spring force

The skin is held together by springs along every unique edge of the mesh (each edge is shared by two triangles but gets only one spring). For an edge between vertices $i$ and $j$, calculate $\vec{d} = \vec{p}_j - \vec{p}_i$, $L = |\vec{d}|$, and $\hat{d} = \vec{d}/L$. The spring force with damping is:

<p></p>

$$ \vec{F}_s = [k_s(L - L_0) + k_d(\vec{v}_j - \vec{v}_i) \cdot \hat{d}]\hat{d} $$

<p>Where:</p>

1. $k_s$ is the spring constant.

2. $k_d$ is the damping constant. The dot product keeps only the relative velocity along the edge, so only the change in edge length is damped.

3. $L_0$ is the rest length of that edge. It is stored once when the mesh is built (the initial length of each edge) and never changes. $L$ is recomputed from the current positions every substep.

<p></p>

<p>The force is applied equal and opposite to the two endpoints (Newton's third law):</p>

$$ \vec{F}_i \mathrel{+}= \vec{F}_s \qquad \vec{F}_j \mathrel{-}= \vec{F}_s $$

<p></p>

### Smoothing force (optional)

To stop the skin from crumpling, each vertex can be pulled toward the average position of its neighbors. If $\mathcal{N}(i)$ is the set of vertices connected to $i$ by an edge:

<p></p>

$$ \vec{F}_m = k_b\left(\frac{1}{|\mathcal{N}(i)|}\sum_{j \in \mathcal{N}(i)} \vec{p}_j - \vec{p}_i\right) $$

<p></p>

This acts on one vertex only, so it is not equal and opposite. On a sphere the neighbor average sits slightly inside the surface, so it shrinks the equilibrium size a little. Keep $k_b$ small or leave it out, since the triangulated edge springs already resist shearing.

<p></p>

### Gravity, buoyancy and drag

<p>These are external forces. They act on each vertex individually, so they are added once per vertex, not once per spring.</p>

$$ \vec{F}_g = m_i\vec{g}, \qquad \vec{g} = (0, -9.81, 0) \ m/s^2 $$

<p></p>

Because $P_{ext}$ is constant, the outside air pushes equally on every triangle and the net force cancels, so there is no lift. Buoyancy has to be added explicitly, using the volume from above and spread over the $N$ vertices:

<p></p>

$$ \vec{F}_b = \frac{\rho_{air} V g}{N}\hat{y} \qquad (\rho_{air} \approx 1.2 \ kg/m^3) $$

<p></p>

The mass of the gas is not on the mesh, so it is added to the vertex masses: $m_i = (m_{skin} + m_{gas})/N$. Then the gas weight and the buoyancy both act on the balloon, and a gas lighter than air rises while a heavier gas sinks.

<p>Drag (linear, to reach a terminal velocity):</p>

$$ \vec{F}_{d} = -c\vec{v}_i $$

<p></p>

### Integration

<p>Every vertex ends up with one net force, the sum of everything accumulated on it this substep:</p>

$$ \vec{F}_{net,i} = \vec{F}_{pressure,i} + \vec{F}_{s,i} + \vec{F}_{m,i} + \vec{F}_{g,i} + \vec{F}_{b,i} + \vec{F}_{d,i} $$

<p></p>

Then, using semi-implicit Euler with a small $Δt$ (several substeps per frame):

<p></p>

$$ \vec{v}_i \leftarrow \vec{v}_i + \frac{\vec{F}_{net,i}}{m_i}Δt $$

$$ \vec{p}_i \leftarrow \vec{p}_i + \vec{v}_iΔt $$

<p>The new velocity is used to update the position.</p>

<p></p>

<p></p>

### Critical point

The critical point is not needed by the simulation loop, only $a$ and $b$ are. It is useful as a phase indicator and a sanity check:

<p></p>

$$ T_c = \frac{8a}{27Rb} \qquad P_c = \frac{a}{27b^2} \qquad V_c = 3nb $$

<p></p>

Above $T_c$ the fluid responds smoothly. Below $T_c$, $P(V)$ has a region where pressure rises with volume, which is unstable, so the balloon can pop or collapse. A pressure clamp or a Maxwell construction is needed there. Near $T_c$ the balloon is very floppy, since a tiny change in pressure causes a large change in volume.

<p></p>

### Table of $a$, $b$ values for the Van der Waals equation

<p></p>

Table of gases and their $a$ and $b$ values (convert to SI before use, see above).

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
| Ozone | O₃ | 3.545 | 0.0487 |
| Nitrous Oxide | N₂O | 3.832 | 0.04415 |
| Hydrogen Sulfide | H₂S | 4.544 | 0.04339 |
| Phosgene | COCl₂ | 9.171 | 0.0831 |
| Ethylene | C₂H₄ | 4.612 | 0.0582 |
| Acetylene | C₂H₂ | 4.516 | 0.0522 |
| Propane | C₃H₈ | 8.779 | 0.08445 |
| Tungsten Hexafluoride | WF₆ | 13.25 | 0.1063
| Butane | C₄H₁₀ | 14.66 | 0.1226 |
| Uranium Hexafluoride | UF₆ | 16.01 | 0.1128 |
| Pentane | C₅H₁₂ | 19.26 | 0.146 |
| Hexane | C₆H₁₄ | 24.71 | 0.1735 |
| Heptane | C₇H₁₆ | 31.06 | 0.2049 |
| Octane | C₈H₁₈ | 37.88 | 0.2374 |