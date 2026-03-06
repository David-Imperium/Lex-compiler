import { CodeTransformer } from './components/CodeTransformer'
import './index.css'

function App() {
  return (
    <div className="app">
      <header className="app-header">
        <h1>Lex Compiler</h1>
        <p>Declarative Game Content Language</p>
      </header>
      <main>
        <CodeTransformer />
      </main>
      <footer className="app-footer">
        <p>
          <a href="https://github.com/David-Imperium/Lex" target="_blank" rel="noopener noreferrer">
            View on GitHub
          </a>
        </p>
      </footer>
    </div>
  )
}

export default App
