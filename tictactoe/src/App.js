import { useState } from 'react';
import ServerButton from './server'; 
import ActiveTab  from './tab';
import StartNewGame from './game';
import Button from '@mui/material/Button';
import LoginButton from './login';
import RegisterButton from './register';
import CommentSection from './commentSection';
import { Box } from '@mui/material';


function Square({ value, onSquareClick }) {
  return (
    <button className="square bg-white border border-gray-300 text-xl font-bold w-15 h-15 btn btn-outline-danger" onClick={onSquareClick} 
    style={{height:'50px', width:'50px'} }>
      {value}
    </button>
  );
}

function Title(){
  return (
    <h1>Tic Tac Toe </h1>
  )
}

function Board({ xIsNext, squares, onPlay }) {
  function handleClick(i) {
    if (calculateWinner(squares) || squares[i]) {
      return;
    }
    const nextSquares = squares.slice();
    if (xIsNext) {
      nextSquares[i] = 'X';
    } else {
      nextSquares[i] = 'O';
    }
    onPlay(nextSquares);
  }

  const winner = calculateWinner(squares);
  let status;
  if (winner) {
    status = 'Winner: ' + winner;
  } else {
    status = 'Next player: ' + (xIsNext ? 'X' : 'O');
  }

  return (
    <>
    <div className="status mb-4">{status}</div>
    <div className="container">
      <div className="row justify-content-center">
        <div className="col-md-6">
          <div className="board">
            <div className="board-row">
              <Square value={squares[0]} onSquareClick={() => handleClick(0)} />
              <Square value={squares[1]} onSquareClick={() => handleClick(1)} />
              <Square value={squares[2]} onSquareClick={() => handleClick(2)} />
            </div>
            <div className="board-row">
              <Square value={squares[3]} onSquareClick={() => handleClick(3)} />
              <Square value={squares[4]} onSquareClick={() => handleClick(4)} />
              <Square value={squares[5]} onSquareClick={() => handleClick(5)} />
            </div>
            <div className="board-row">
              <Square value={squares[6]} onSquareClick={() => handleClick(6)} />
              <Square value={squares[7]} onSquareClick={() => handleClick(7)} />
              <Square value={squares[8]} onSquareClick={() => handleClick(8)} />
            </div>
          </div>
        </div>
      </div>
    </div>
  </>
  
  );
}

function Game() {
  const [history, setHistory] = useState([Array(9).fill(null)]);
  const [currentMove, setCurrentMove] = useState(0);
  const xIsNext = currentMove % 2 === 0;
  const currentSquares = history[currentMove];

  function handlePlay(nextSquares) {
    const nextHistory = [...history.slice(0, currentMove + 1), nextSquares];
    setHistory(nextHistory);
    setCurrentMove(nextHistory.length - 1);
  }

  function jumpTo(nextMove) {
    setCurrentMove(nextMove);
  }

  function handleReset() {
    setHistory([Array(9).fill(null)]);
    setCurrentMove(0);
  }
  function ResetButton() {
    return (
      <Button variant='contained' color='error' onClick={handleReset} style={{'marginTop': '5px'}}> 
        Reset
      </Button>
    )
  }
  // const moves = history.map((squares, move) => {
  //   let description;
  //   if (move > 0) {
  //     description = 'Go to move #' + move;
  //   } else {
  //     description = 'Go to game start';
  //   }
  //   return (
  //     <li key={move}>
  //       <button onClick={() => jumpTo(move)} className="text-blue-500 hover:underline">{description}</button>
  //     </li>
  //   );
  // });

  const [comments, setComments] = useState([
    {'name': 'saransh', 'msg': 'kasto chak jasto game'}
  ]);
  const [currentUser, setCurrentUser] = useState('saransh')
  const addComment = (name, message) => {
    const newComment = { name, 'msg': message };
    setComments([...comments, newComment]);
};

return (
    <div className="game" style={{
      display: 'flex', flexDirection: 'column'
    }}>
      <Title />
      <ServerButton />
      <div className="flex">
      <LoginButton /> 
       <RegisterButton />

      </div>
      {/* <StartNewGame /> */}
      <div className="game-board">
        <Board xIsNext={xIsNext} squares={currentSquares} onPlay={handlePlay} />
      </div>
      {/* <ResetButton /> */}
      <Box sx={{position: 'absolute', left: '10px', top: '50px', padding: '2em', background: 'whitesmoke' ,borderRadius: '2px'}}>
        <CommentSection gameNumber ={1141} comments={comments} addComment={addComment} currentUser={currentUser}/>

      </Box>
      <ActiveTab />
    </div>
  );
}

function calculateWinner(squares) {
  const lines = [
    [0, 1, 2],
    [3, 4, 5],
    [6, 7, 8],
    [0, 3, 6],
    [1, 4, 7],
    [2, 5, 8],
    [0, 4, 8],
    [2, 4, 6],
  ];
  for (let i = 0; i < lines.length; i++) {
    const [a, b, c] = lines[i];
    if (squares[a] && squares[a] === squares[b] && squares[a] === squares[c]) {
      return squares[a];
    }
  }
  return null;
}

export default Game;
