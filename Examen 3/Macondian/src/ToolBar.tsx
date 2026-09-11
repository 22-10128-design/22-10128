///
/// ToolBar.tsx
///

import React from 'react';
import { Button, ButtonGroup } from 'react-bootstrap';

interface Props {
  start: () => void;
  reset: () => void;
  uxColor: (mode: number) => string;
  setUX: (mode: number) => void;
  error: string;
}

const ToolBar: React.FC<Props> = ({ start, reset, uxColor, setUX, error }) => {
  return (
    <div className="app-toolbar d-flex justify-content-between align-items-center p-2 bg-dark text-white">
      <ButtonGroup>
        <Button variant="success" onClick={start}>Start Macondian</Button>
        <Button variant="danger" onClick={reset}>Reset</Button>
      </ButtonGroup>

      <ButtonGroup className="ms-3">
        <Button variant={uxColor(0) === 'Yellow' ? 'warning' : 'secondary'} onClick={() => setUX(0)}>
          Raw
        </Button>
        <Button variant={uxColor(1) === 'Yellow' ? 'warning' : 'secondary'} onClick={() => setUX(1)}>
          Test
        </Button>
        <Button variant={uxColor(2) === 'Yellow' ? 'warning' : 'secondary'} onClick={() => setUX(2)}>
          Chart
        </Button>
        <Button variant={uxColor(3) === 'Yellow' ? 'warning' : 'secondary'} onClick={() => setUX(3)}>
          Image
        </Button>
        <Button variant={uxColor(4) === 'Yellow' ? 'warning' : 'secondary'} onClick={() => setUX(4)}>
          List
        </Button>
      </ButtonGroup>

      {error && <span className="text-danger ms-auto">{error}</span>}
    </div>
  );
};

export default ToolBar;