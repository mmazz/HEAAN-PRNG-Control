/*
 * FaultHook.h -- punto de extension para inyectores de fallas.
 *
 * Este fork expone puntos de inyeccion internos a las operaciones. Antes cada
 * uno llamaba a NTL::SwitchBit() directamente; eso tenia dos problemas:
 *
 *   1. Escribir en poly[coeff] con coeff >= poly.rep.length() es UB en NTL:
 *      el bit se pierde y se pisa memoria que no pertenece al polinomio.
 *   2. Un framework externo no puede enterarse de si el flip se aplico o no,
 *      asi que una campania podia reportar 0% SDC en silencio.
 *
 * Ahora TODOS los puntos de inyeccion pasan por heaanfi::flip(). El default
 * (default_flip) hace lo mismo que antes pero con SetLength/normalize
 * defensivos, asi que el fork sigue siendo usable solo. Un framework externo
 * llama a set_flip() una vez al arrancar y a partir de ahi ve cada flip.
 *
 * NO se incluye nada del framework aca a proposito: el fork no debe depender
 * de fi-ckks.
 */
#ifndef HEAAN_FAULTHOOK_H_
#define HEAAN_FAULTHOOK_H_

#include <NTL/ZZX.h>
#include <cstdint>

namespace heaanfi {

// Voltea `width` bits contiguos desde `bit` en el coeficiente `coeff`.
// ring_degree = context.N cuando el llamador lo conoce, 0 si no.
// Hace falta porque HEAAN indexa poly.rep[i] para todo i < N SIN mirar
// rep.length(): la biblioteca entera asume ZZX de largo exactamente N.
// Por eso el hook estira hasta N y NUNCA llama a normalize(): un normalize
// que recorte el polinomio deja a Ring2Utils leyendo fuera del vector.
using FlipFn = void (*)(NTL::ZZX& poly,
                        std::uint32_t coeff,
                        std::uint32_t bit,
                        std::uint32_t width,
                        long ring_degree);

// Implementacion por defecto: SetLength defensivo + SwitchBit + normalize.
void default_flip(NTL::ZZX& poly,
                  std::uint32_t coeff,
                  std::uint32_t bit,
                  std::uint32_t width,
                  long ring_degree);

extern FlipFn g_flip;

// Instala una implementacion propia. Devuelve la anterior.
// nullptr restaura default_flip. Se llama UNA vez al arrancar, antes de
// lanzar hilos: g_flip no esta protegido.
FlipFn set_flip(FlipFn fn);

inline void flip(NTL::ZZX& poly,
                 std::uint32_t coeff,
                 std::uint32_t bit,
                 std::uint32_t width,
                 long ring_degree)
{
    g_flip(poly, coeff, bit, width, ring_degree);
}

} // namespace heaanfi

#endif // HEAAN_FAULTHOOK_H_
