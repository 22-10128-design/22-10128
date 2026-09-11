///
/// Macondian.tsx
///

import { SubmitEvent, useEffect, useRef, useState } from 'react';
import { Button, ButtonGroup } from 'react-bootstrap';

import Chart, { processMacondianSensor, SensorBatchData } from "./Chart";
import CLI from "./CLI";
import Image from "./Image";
import List from "./List";
import Monitor from "./Monitor";
import ToolBar from "./ToolBar";

import 'bootstrap/dist/css/bootstrap.min.css';

const project = "ZenSheet™";
const artefact = "Macondian Simulator";
const version = "V-20260901";

const title = <h3>The Great {artefact}</h3>;
const product = <em><b>{artefact}</b> Interactive Computing Environment</em>;
const copyright = <>Copyright © <b>Lakebolt™ Research</b> 2024-2026</>;

const MONITOR_SIZE = 1024;

const ART_IMAGES = [
  "ART042.jpg",
  "ART067.png",
  "ART097.png",
  "ART112.png",
  "ART127.png",
  "ART409.png"
];

const MAC_IMAGES = [
  "MAC091.png",
  "MAC092.png",
  "MAC127.png",
  "MAC206.png",
  "MAC224.png",
  "MAC997.jpg"
];

const ALL_IMAGES = [...ART_IMAGES, ...MAC_IMAGES];

const tgmr = new Worker(new URL("./Macondian/tgmr-thx-1138.js", import.meta.url));

const Macondian = () => {

  const command = useRef<HTMLInputElement>(null);
  const [rawLog, setRawLog] = useState<string[]>([]);
  const [seriesLog, setSeriesLog] = useState<string[]>([]);
  const [chartData, setChartData] = useState<SensorBatchData[]>([]);
  const currentBatchRef = useRef<number>(0);

 
  const [ux, setUX] = useState(0);

 
  const [imageSubMode, setImageSubMode] = useState<'single' | 'double'>('single');


  const [singleIndex, setSingleIndex] = useState(0);
  const [artIndex, setArtIndex] = useState(0);
  const [macIndex, setMacIndex] = useState(0);

 
  useEffect(() => {
    if (ux !== 3) return;

    if (imageSubMode === 'single') {
      const timer = setInterval(() => {
        setSingleIndex(prev => (prev + 1) % ALL_IMAGES.length);
      }, 2000);
      return () => clearInterval(timer);
    }

  
    const timerDouble = setInterval(() => {
      setArtIndex(prev => (prev + 1) % ART_IMAGES.length);
      setMacIndex(prev => (prev + 1) % MAC_IMAGES.length);
    }, 2000);

    return () => clearInterval(timerDouble);
  }, [ux, imageSubMode]);

  const uxColor = (mode: number) => {
    return mode === ux ? "Yellow" : "Gray";
  };

  const [error] = useState<string>('');

  const [leftPct, setLeftPct] = useState(50);
  const splitRef = useRef<HTMLDivElement>(null);
  const dragging = useRef(false);

  useEffect(() => {
    const onMove = (event: MouseEvent) => {
      if (!dragging.current || !splitRef.current) return;
      const rect = splitRef.current.getBoundingClientRect();
      const pct = ((event.clientX - rect.left) / rect.width) * 100;
      setLeftPct(Math.min(80, Math.max(20, pct)));
    };
    const onUp = () => {
      dragging.current = false;
      document.body.style.userSelect = '';
      document.body.style.cursor = '';
    };
    window.addEventListener('mousemove', onMove);
    window.addEventListener('mouseup', onUp);
    return () => {
      window.removeEventListener('mousemove', onMove);
      window.removeEventListener('mouseup', onUp);
    };
  }, []);

  const startDrag = (event: React.MouseEvent) => {
    event.preventDefault();
    dragging.current = true;
    document.body.style.userSelect = 'none';
    document.body.style.cursor = 'col-resize';
  };

  const addHistoryItem = (item: string) => {
    setRawLog(h => {
      const next = h.length < MONITOR_SIZE ? [...h, item] : [...h.slice(1), item];
      return next;
    });

    parseAndProcessMessage(item);
  };

  const parseAndProcessMessage = (msg: string) => {
    if (msg.startsWith("Batch ")) {
      const match = msg.match(/Batch (\d+):/);
      if (match) {
        currentBatchRef.current = parseInt(match[1], 10);
        setSeriesLog(s => [...s, `--- Batch ${currentBatchRef.current} ---`]);
      }
      return;
    }

    const sensorRegex = /(Y\d+|L\d+)\s+\[\s*([\d\.\,\s\-]+)\s*\]/;
    const sensorMatch = msg.match(sensorRegex);

    if (sensorMatch) {
      const sensorId = sensorMatch[1];
      const rawValues = sensorMatch[2]
        .split(',')
        .map(v => parseFloat(v.trim()))
        .filter(v => !isNaN(v));

      if (rawValues.length > 0) {
        const macondoVal = processMacondianSensor(rawValues, 0.08);
        const logFormatted = `${sensorId} -> Macondo: ${macondoVal.toFixed(3)}`;
        setSeriesLog(s => [...s, logFormatted]);

        setChartData(c => [
          ...c,
          {
            batchId: currentBatchRef.current,
            sensorId,
            rawMicroarray: rawValues,
            macondoValue: macondoVal,
          }
        ]);
      }
    }
  };

  useEffect(() => {
    tgmr.onmessage = (envelope: MessageEvent) => {
      const data = envelope.data;
      if (!data) {
        addHistoryItem(`ERROR: bad data in ${envelope}`);
        return;
      }
      addHistoryItem(data);
    };
  }, []);

  const send = (request: string) => {
    tgmr.postMessage(`${request}`);
  };

  const clearLogs = () => {
    setRawLog([]);
    setSeriesLog([]);
    setChartData([]);
    currentBatchRef.current = 0;
  };

  const start = () => send(".start");
  const reset = () => {
    send(".reset");
    setTimeout(clearLogs, 496);
  };

  const nop = () => {};

  const kvp = [
    { key: ".start", fun: start },
    { key: ".reset", fun: reset },
  ];

  const map = (key: string) => {
    for (let element of kvp) {
      if (key === element.key) return element.fun;
    }
    return nop;
  };

  const cliRequest = (event: SubmitEvent<HTMLFormElement>) => {
    event.preventDefault();
    if (command.current) {
      const request = command.current.value;
      command.current.value = "";
      map(request)();
    }
  };

  
  const renderCenteredImage = (imgName: string) => (
    <div style={{
      display: 'flex',
      justifyContent: 'center',
      alignItems: 'center',
      width: '100%',
      height: '100%',
      overflow: 'hidden'
    }}>
      <Image image={imgName} />
    </div>
  );

  

  const rawView = (
    <div className="app-split" ref={splitRef}>
      <section className="app-pane" data-bs-theme="dark" style={{ flexGrow: leftPct }}>
        <Monitor title={"señal cruda"} log={rawLog} />
      </section>
      <div className="pane-divider" onMouseDown={startDrag} title="Drag to resize" />
      <section className="app-pane" style={{ flexGrow: 100 - leftPct }}>
        {renderCenteredImage("MAC997.jpg")}
      </section>
    </div>
  );

  const testView = (
    <div className="app-split" ref={splitRef}>
      <section className="app-pane" data-bs-theme="dark" style={{ flexGrow: leftPct }}>
        <Monitor title={"señal cruda"} log={rawLog} />
      </section>
      <div className="pane-divider" onMouseDown={startDrag} title="Drag to resize" />
      <section className="app-pane" style={{ flexGrow: 100 - leftPct }}>
        <Monitor title={"serie procesada"} log={seriesLog} />
      </section>
    </div>
  );

  const chartView = (
    <div className="app-split" ref={splitRef}>
      <section className="app-pane" data-bs-theme="dark" style={{ flexGrow: leftPct }}>
        <Monitor title={"señal cruda"} log={rawLog} />
      </section>
      <div className="pane-divider" onMouseDown={startDrag} title="Drag to resize" />
      <section className="app-pane" style={{ flexGrow: 100 - leftPct }}>
        <Chart data={chartData} />
      </section>
    </div>
  );

  
  const imageView = (
    <div style={{ display: 'flex', flexDirection: 'column', width: '100%', height: '100%' }}>
      <div className="d-flex justify-content-center p-2 bg-secondary">
        <ButtonGroup size="sm">
          <Button
            variant={imageSubMode === 'single' ? 'warning' : 'dark'}
            onClick={() => setImageSubMode('single')}
          >
            Carousel
          </Button>
          <Button
            variant={imageSubMode === 'double' ? 'warning' : 'dark'}
            onClick={() => setImageSubMode('double')}
          >
            Double
          </Button>
        </ButtonGroup>
      </div>

      <div style={{ flexGrow: 1, position: 'relative', overflow: 'hidden' }}>
        {imageSubMode === 'single' ? (
          renderCenteredImage(ALL_IMAGES[singleIndex])
        ) : (
          <div className="app-split" ref={splitRef} style={{ height: '100%' }}>
            <section className="app-pane" style={{ flexGrow: leftPct }}>
              {renderCenteredImage(ART_IMAGES[artIndex])}
            </section>
            <div className="pane-divider" onMouseDown={startDrag} title="Drag to resize" />
            <section className="app-pane" style={{ flexGrow: 100 - leftPct }}>
              {renderCenteredImage(MAC_IMAGES[macIndex])}
            </section>
          </div>
        )}
      </div>
    </div>
  );

  const listView = (
    <List />
  );

  const views = [ rawView, testView, chartView, imageView, listView ];

  return (
    <div className="app-shell">
      <header className="app-header">
        <>{title}</>
      </header>
      <ToolBar
        start={start}
        reset={reset}
        uxColor={uxColor}
        setUX={setUX}
        error={error}
      />
      <main className="app-main">
        { views[ux] }
      </main>
      <CLI req={cliRequest} ref={command} />
      <footer className="app-footer">
        <h6><b>{project}</b> Project: {product} {version} - {copyright} </h6>
      </footer>
    </div>
  );
}

export default Macondian;