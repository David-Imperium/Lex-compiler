import { useEffect, useRef, useState } from 'react';
import Prism from 'prismjs';
import 'prismjs/components/prism-lua';
import 'prismjs/components/prism-json';
import 'prismjs/components/prism-csharp';
import 'prismjs/components/prism-javascript';
import 'prismjs/themes/prism-tomorrow.css';

interface CodeBlockProps {
  code: string;
  language: 'lex' | 'lua' | 'json' | 'csharp' | 'gdscript';
  title?: string;
  typing?: boolean;
  typingSpeed?: number;
  onTypingComplete?: () => void;
}

// Custom Lex language definition
Prism.languages.lex = {
  'comment': /\/\/.*|\/\*[\s\S]*?\*\//,
  'keyword': /\b(era|structure|unit|technology|resource|event|condition|effect|import|export|as|public|internal|hidden)\b/,
  'property': /\b(name|cost|requires|unlocks|produces|type|health|damage|speed|range|description)\b/,
  'string': /"(?:[^"\\]|\\.)*"/,
  'number': /\b\d+\.?\d*\b/,
  'color': /#[0-9a-fA-F]{6}\b/,
  'operator': /[:{}\[\],]/,
  'boolean': /\b(true|false)\b/,
};

Prism.languages.gdscript = Prism.languages.extend('javascript', {
  'keyword': /\b(func|var|const|class|extends|export|signal|enum|static|pass|return|if|elif|else|for|while|match|break|continue|class_name|setget|onready|tool|preload|yield|assert|breakpoint|remote|master|puppet|remotesync|mastersync|puppetsync)\b/,
});

export function CodeBlock({
  code,
  language,
  title,
  typing = false,
  typingSpeed = 30,
  onTypingComplete
}: CodeBlockProps) {
  const [displayedCode, setDisplayedCode] = useState('');
  const [showCursor, setShowCursor] = useState(false);
  const codeRef = useRef<HTMLElement>(null);

  useEffect(() => {
    if (!typing) {
      setDisplayedCode(code);
      setShowCursor(false);
      return;
    }

    setDisplayedCode('');
    setShowCursor(true);
    let index = 0;
    let timeout: ReturnType<typeof setTimeout>;

    const typeNextChar = () => {
      if (index < code.length) {
        setDisplayedCode(code.slice(0, index + 1));
        index++;

        // Randomize speed for input (if typingSpeed > 10 it's human)
        const delay = typingSpeed <= 10
          ? typingSpeed
          : typingSpeed + (Math.random() * typingSpeed - typingSpeed * 0.5);

        timeout = setTimeout(typeNextChar, delay);
      } else {
        setShowCursor(false);
        onTypingComplete?.();
      }
    };

    timeout = setTimeout(typeNextChar, typingSpeed);

    return () => clearTimeout(timeout);
  }, [code, typing, typingSpeed, onTypingComplete]);

  useEffect(() => {
    if (codeRef.current) {
      Prism.highlightElement(codeRef.current);
    }
  }, [displayedCode, language]);

  return (
    <div className="code-block" style={{ display: 'flex', flexDirection: 'column', height: '100%' }}>
      {title && (
        <div className="code-header" style={{ display: 'flex', alignItems: 'center', gap: '12px', padding: '12px 16px', background: '#121217', borderBottom: '1px solid #334155' }}>
          <div style={{ display: 'flex', gap: '6px' }}>
            <div style={{ width: 10, height: 10, borderRadius: '50%', background: '#334155' }}></div>
            <div style={{ width: 10, height: 10, borderRadius: '50%', background: '#334155' }}></div>
            <div style={{ width: 10, height: 10, borderRadius: '50%', background: '#334155' }}></div>
          </div>
          <span className="code-title" style={{ color: '#94a3b8', fontSize: '0.8rem', fontFamily: '"JetBrains Mono", monospace' }}>{title}</span>
        </div>
      )}
      <pre className="code-content" style={{ background: '#000', margin: 0, padding: '24px', flexGrow: 1, overflowX: 'auto', fontSize: '0.85rem', fontFamily: '"JetBrains Mono", monospace', color: '#e2e8f0', border: '1px solid #334155', borderTop: 'none' }}>
        <code ref={codeRef} className={`language-${language}`}>
          {displayedCode}
        </code>
        {showCursor && <span className="blinking-cursor" style={{ color: '#00f2fe', animation: 'blink 1s step-end infinite' }}>▋</span>}
      </pre>
      <style>{`
        @keyframes blink { 50% { opacity: 0; } }
      `}</style>
    </div>
  );
}
