///
/// Chart
///

export interface SensorBatchData {
  batchId: number;
  sensorId: string;
  rawMicroarray: number[];
  macondoValue: number; // Resultado del filtro
}

interface Props {
  data?: SensorBatchData[];
}

// Algoritmo de filtrado por radio de tolerancia
export const processMacondianSensor = (rawValues: number[], toleranceRadio: number = 0.08): number => {
  if (!rawValues || rawValues.length === 0) return 0;

  // 1. Calcular la mediana
  const sorted = [...rawValues].sort((a, b) => a - b);
  const mid = Math.floor(sorted.length / 2);
  const median = sorted.length % 2 !== 0 ? sorted[mid] : (sorted[mid - 1] + sorted[mid]) / 2;

  // 2. Filtrar outliers según el radio de tolerancia (8%)
  const maxDiff = Math.abs(median) * toleranceRadio;
  const filtered = rawValues.filter(val => Math.abs(val - median) <= maxDiff);

  // 3. Promedio de valores válidos
  const validArray = filtered.length > 0 ? filtered : rawValues;
  const sum = validArray.reduce((acc, curr) => acc + curr, 0);
  return sum / validArray.length;
};

const Chart = ({ data = [] }: Props) => {
  if (!data || data.length === 0) {
    return (
      <div style={{ padding: "20px", color: "#888", textAlign: "center" }}>
        <h3>Esperando flujo de datos del simulador Macondian...</h3>
      </div>
    );
  }

  // Agrupar lecturas procesadas por Batch
  const batchesMap = new Map<number, number[]>();
  data.forEach(item => {
    if (!batchesMap.has(item.batchId)) batchesMap.set(item.batchId, []);
    batchesMap.get(item.batchId)?.push(item.macondoValue);
  });

  const batchIds = Array.from(batchesMap.keys()).sort((a, b) => a - b);
  const averages = batchIds.map(id => {
    const vals = batchesMap.get(id) || [0];
    return vals.reduce((a, b) => a + b, 0) / vals.length;
  });

  const maxVal = Math.max(...averages, 1);
  const minVal = Math.min(...averages, 0);

  return (
    <div style={{ padding: "20px", background: "#1a1a1a", color: "#fff", borderRadius: "8px" }}>
      <h3 style={{ marginTop: 0, color: "#4da6ff" }}>Serie de Tiempo: Respuesta Macondiana Procesada</h3>
      
      <svg width="100%" height="300" viewBox="0 0 600 300" style={{ background: "#111", borderRadius: "6px" }}>
        {/* Ejes y Cuadrícula */}
        <line x1="50" y1="20" x2="50" y2="250" stroke="#444" strokeWidth="2" />
        <line x1="50" y1="250" x2="570" y2="250" stroke="#444" strokeWidth="2" />

        {/* Puntos y Línea de Tendencia */}
        {batchIds.map((batch, index) => {
          const x = 50 + (index / Math.max(batchIds.length - 1, 1)) * 520;
          const normalizedY = (averages[index] - minVal) / Math.max(maxVal - minVal, 0.001);
          const y = 250 - normalizedY * 210;

          const nextX = index < batchIds.length - 1 ? 50 + ((index + 1) / Math.max(batchIds.length - 1, 1)) * 520 : null;
          const nextNormY = index < batchIds.length - 1 ? (averages[index + 1] - minVal) / Math.max(maxVal - minVal, 0.001) : null;
          const nextY = nextNormY !== null ? 250 - nextNormY * 210 : null;

          return (
            <g key={batch}>
              {nextX !== null && nextY !== null && (
                <line x1={x} y1={y} x2={nextX} y2={nextY} stroke="#00e676" strokeWidth="2" />
              )}
              <circle cx={x} cy={y} r="5" fill="#ff4081" />
              <text x={x - 10} y="270" fill="#aaa" fontSize="10">B{batch}</text>
              <text x={x - 12} y={y - 10} fill="#fff" fontSize="10">{averages[index].toFixed(2)}</text>
            </g>
          );
        })}
      </svg>
    </div>
  );
};

export default Chart;