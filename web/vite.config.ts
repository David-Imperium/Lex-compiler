import { defineConfig } from 'vite'
import react from '@vitejs/plugin-react'

// https://vite.dev/config/
export default defineConfig({
  plugins: [react()],
  server: {
    port: 3173, // Porta dedicata per Lex demo
    strictPort: true,
  },
  base: '/Lex-compiler/', // Base path per GitHub Pages
})
