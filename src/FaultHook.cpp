#include "FaultHook.h"

#include <NTL/ZZ.h>
#include <algorithm>

namespace heaanfi {

void default_flip(NTL::ZZX& poly,
                  std::uint32_t coeff,
                  std::uint32_t bit,
                  std::uint32_t width,
                  long ring_degree)
{
    const long c = static_cast<long>(coeff);

    // ZZX guarda solo hasta el grado efectivo. poly[c] con c >= rep.length()
    // NO es un cero: NTL devuelve una referencia a memoria que no forma parte
    // del polinomio, asi que el flip se pierde y se pisa lo que haya al lado.
    // Hay que estirar antes de escribir.
    //
    // Se estira hasta max(c+1, N) y NO se normaliza. HEAAN recorre
    // poly.rep[i] para todo i < N sin mirar length() (ver Ring2Utils), asi
    // que un polinomio mas corto que N es un segfault esperando, y
    // normalize() es justo la operacion que lo acorta.
    const long need = std::max<long>(c + 1, ring_degree);
    if (poly.rep.length() < need)
        poly.SetLength(need);

    for (std::uint32_t i = 0; i < width; ++i)
        NTL::SwitchBit(poly.rep[c], static_cast<long>(bit + i));
}

FlipFn g_flip = &default_flip;

FlipFn set_flip(FlipFn fn)
{
    FlipFn old = g_flip;
    g_flip = fn ? fn : &default_flip;
    return old;
}

} // namespace heaanfi
